#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

/*
 * Reading validation. The sensor tasks must only send a reading when its CRC checks out,
 * so a dead or noisy sensor stays silent (that is what makes staleness detection work).
 *
 * Expects main/data/crc.h and crc.c:
 *
 *   uint8_t crc8_sht31(const uint8_t *data, size_t len);     // SHT3x: poly 0x31, init 0xFF, MSB first
 *   uint8_t crc8_ds18b20(const uint8_t *data, size_t len);   // Dallas/Maxim: poly 0x8C reflected, init 0
 *   bool ds18b20_scratchpad_valid(const uint8_t scratchpad[9]);
 *       true only for a real scratchpad: CRC over all 9 bytes is 0 AND it is not what a dead bus reads.
 *
 * Reference values come from the datasheets, so they are facts, not design choices.
 * The test Makefile already lists crc_tdd.cpp and ../main/data/crc.c.
 */

extern "C" {
    #include "../main/data/crc.h"
}

TEST_GROUP(CrcGroup)
{
};

// Pins down: the SHT3x datasheet example. The two bytes 0xBE 0xEF have CRC 0x92.
TEST(CrcGroup, Sht31CrcMatchesTheDatasheetExample)
{
    const uint8_t word[] = {0xBE, 0xEF};

    LONGS_EQUAL(0x92, crc8_sht31(word, sizeof(word)));
}

// Pins down: a flipped bit in the data changes the CRC, so corruption is caught.
TEST(CrcGroup, Sht31CrcChangesWhenADataBitIsFlipped)
{
    const uint8_t good[] = {0xBE, 0xEF};
    const uint8_t corrupted[] = {0xBE, 0xEE};

    CHECK(crc8_sht31(good, sizeof(good)) != crc8_sht31(corrupted, sizeof(corrupted)));
}

// Pins down: the DS18B20 datasheet scratchpad (85 C power-on value) has CRC 0x1C over its first 8 bytes.
TEST(CrcGroup, Ds18b20CrcMatchesTheDatasheetScratchpad)
{
    const uint8_t scratchpad[] = {0x50, 0x05, 0x4B, 0x46, 0x7F, 0xFF, 0x0C, 0x10};

    LONGS_EQUAL(0x1C, crc8_ds18b20(scratchpad, sizeof(scratchpad)));
}

// Pins down: running the CRC over all 9 scratchpad bytes (CRC included) gives 0 for a good read.
// This is the one-line validity check the task can use after reading the scratchpad.
TEST(CrcGroup, Ds18b20CrcOverAllNineBytesIsZeroForAGoodRead)
{
    const uint8_t scratchpad[] = {0x50, 0x05, 0x4B, 0x46, 0x7F, 0xFF, 0x0C, 0x10, 0x1C};

    LONGS_EQUAL(0, crc8_ds18b20(scratchpad, sizeof(scratchpad)));
}

// Pins down: the two dead-bus readings and what the CRC rule makes of them.
// An empty bus reads back all 0xFF (the pull-up holds the line high). A bus stuck low reads all 0x00.
// The CRC over nine 0xFF bytes is 0x63, so "CRC == 0" already rejects an empty bus.
// The CRC over nine 0x00 bytes is 0, so "CRC == 0" alone WRONGLY accepts a stuck-low bus.
TEST(CrcGroup, ACrcOfZeroAloneWronglyAcceptsAStuckLowBus)
{
    const uint8_t empty_bus[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    const uint8_t stuck_low[9] = {0};

    LONGS_EQUAL(0x63, crc8_ds18b20(empty_bus, sizeof(empty_bus)));
    LONGS_EQUAL(0, crc8_ds18b20(stuck_low, sizeof(stuck_low)));
}

// Pins down: a real scratchpad is accepted.
TEST(CrcGroup, ARealScratchpadIsValid)
{
    const uint8_t scratchpad[9] = {0x50, 0x05, 0x4B, 0x46, 0x7F, 0xFF, 0x0C, 0x10, 0x1C};

    CHECK_TRUE(ds18b20_scratchpad_valid(scratchpad));
}

// Pins down: a scratchpad with one corrupted byte is rejected.
TEST(CrcGroup, AScratchpadWithACorruptedByteIsNotValid)
{
    const uint8_t scratchpad[9] = {0x50, 0x05, 0x4B, 0x46, 0x7F, 0xFF, 0x0C, 0x11, 0x1C};

    CHECK_FALSE(ds18b20_scratchpad_valid(scratchpad));
}

// Pins down: an empty bus (all 0xFF) is rejected.
TEST(CrcGroup, AnEmptyBusIsNotAValidScratchpad)
{
    const uint8_t empty_bus[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

    CHECK_FALSE(ds18b20_scratchpad_valid(empty_bus));
}

// Pins down: a bus stuck low (all 0x00) is rejected even though its CRC is 0.
TEST(CrcGroup, AStuckLowBusIsNotAValidScratchpad)
{
    const uint8_t stuck_low[9] = {0};

    CHECK_FALSE(ds18b20_scratchpad_valid(stuck_low));
}
