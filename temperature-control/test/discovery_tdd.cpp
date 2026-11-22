#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

/*
 * Sensor discovery. These tests define the API you are going to write in
 * main/discovery/discovery.h and discovery.c:
 *
 *   typedef enum { SENSOR_SHT31 = 1 << 0, SENSOR_DS18B20 = 1 << 1 } sensor_flag_t;
 *   typedef bool (*probe_fn_t)(void);
 *   typedef struct { probe_fn_t sht31; probe_fn_t ds18b20; } discovery_probes_t;
 *
 *   uint8_t discovery_scan(const discovery_probes_t *probes);
 *       Calls each probe once. Returns a bitmask of the sensors that answered.
 *
 *   uint8_t discovery_newly_found(uint8_t already_running, uint8_t found_now);
 *       Returns the sensors that need a new task created for them.
 *
 *   uint8_t discovery_lost(uint8_t already_running, uint8_t found_now);
 *       Returns the sensors that have a task but stopped answering. The discovery
 *       task stops those tasks, and clears them from its "running" mask.
 *
 * The real probes (I2C ack at 0x44, 1-Wire presence pulse) are plugged in on the
 * board. Here they are fakes, so none of this needs hardware.
 *
 * The assertions are written. Decisions behind them: probe once at boot; a missing sensor means
 * no task; a sensor that stops answering has its task stopped, and gets a new one when it
 * comes back; a node with nothing attached keeps probing.
 *
 * Needs two additions to test/Makefile SRCS:
 *   discovery_tdd.cpp \
 *   ../main/discovery/discovery.c
 */

extern "C" {
    #include "../main/discovery/discovery.h"
}

/* ---- fakes: a probe answers whatever these say, and counts how often it was asked ---- */
static bool fake_sht31_present;
static bool fake_ds18b20_present;
static int sht31_probe_calls;
static int ds18b20_probe_calls;

static bool fake_probe_sht31(void)
{
    sht31_probe_calls++;
    return fake_sht31_present;
}

static bool fake_probe_ds18b20(void)
{
    ds18b20_probe_calls++;
    return fake_ds18b20_present;
}

TEST_GROUP(DiscoveryGroup)
{
    discovery_probes_t probes;

    void setup()
    {
        fake_sht31_present = false;
        fake_ds18b20_present = false;
        sht31_probe_calls = 0;
        ds18b20_probe_calls = 0;
        probes.sht31 = fake_probe_sht31;
        probes.ds18b20 = fake_probe_ds18b20;
    }
};

/* ---- discovery_scan ---- */

// Pins down: a bare board with no sensors reports nothing found.
TEST(DiscoveryGroup, ScanReportsNothingWhenNoSensorAnswers)
{
    uint8_t found = discovery_scan(&probes);

    LONGS_EQUAL(0, found);
}

// Pins down: only the SHT31 answering gives only the SHT31 flag.
TEST(DiscoveryGroup, ScanFindsOnlySht31WhenOnlyItAnswers)
{
    fake_sht31_present = true;

    uint8_t found = discovery_scan(&probes);

    LONGS_EQUAL(SENSOR_SHT31, found);
}

// Pins down: only the 1-Wire probe answering gives only the DS18B20 flag.
TEST(DiscoveryGroup, ScanFindsOnlyDs18b20WhenOnlyItAnswers)
{
    fake_ds18b20_present = true;

    uint8_t found = discovery_scan(&probes);

    LONGS_EQUAL(SENSOR_DS18B20, found);
}

// Pins down: both sensors answering are both reported in one result.
TEST(DiscoveryGroup, ScanFindsBothWhenBothAnswer)
{
    fake_sht31_present = true;
    fake_ds18b20_present = true;

    uint8_t found = discovery_scan(&probes);

    LONGS_EQUAL(SENSOR_SHT31 | SENSOR_DS18B20, found);
}

// Pins down: one scan asks each sensor once, not zero times and not repeatedly.
TEST(DiscoveryGroup, ScanProbesEachSensorExactlyOnce)
{
    discovery_scan(&probes);

    LONGS_EQUAL(1, sht31_probe_calls);
    LONGS_EQUAL(1, ds18b20_probe_calls);
}

// Pins down: a node that has no probe for a sensor is treated as not having it.
// Design choice: does a missing probe crash, count as absent, or count as an error?
TEST(DiscoveryGroup, ScanTreatsAMissingProbeFunctionAsSensorAbsent)
{
    probes.ds18b20 = NULL;
    fake_sht31_present = true;

    uint8_t found = discovery_scan(&probes);

    /* A NULL probe counts as "sensor absent", not a crash. The SHT31 is still found. */
    LONGS_EQUAL(SENSOR_SHT31, found);
}

/* ---- discovery_newly_found: what the background probing thread acts on ---- */

// Pins down: when every sensor found already has a task, no new task is created.
TEST(DiscoveryGroup, NewlyFoundIsEmptyWhenEverythingFoundIsAlreadyRunning)
{
    uint8_t running = SENSOR_SHT31 | SENSOR_DS18B20;
    uint8_t found_now = SENSOR_SHT31 | SENSOR_DS18B20;

    uint8_t to_start = discovery_newly_found(running, found_now);

    LONGS_EQUAL(0, to_start);
}

// Pins down: a sensor plugged in later gets a task, and the running one is left alone.
TEST(DiscoveryGroup, NewlyFoundReturnsOnlyTheSensorThatAppeared)
{
    uint8_t running = SENSOR_SHT31;
    uint8_t found_now = SENSOR_SHT31 | SENSOR_DS18B20;

    uint8_t to_start = discovery_newly_found(running, found_now);

    LONGS_EQUAL(SENSOR_DS18B20, to_start);
}

// Pins down: everything found on a later pass is new when nothing is running yet
// (the node booted with nothing attached).
TEST(DiscoveryGroup, NewlyFoundReturnsEverythingWhenNothingWasRunning)
{
    uint8_t running = 0;
    uint8_t found_now = SENSOR_SHT31 | SENSOR_DS18B20;

    uint8_t to_start = discovery_newly_found(running, found_now);

    LONGS_EQUAL(SENSOR_SHT31 | SENSOR_DS18B20, to_start);
}

// Pins down: once the discovery task has stopped a dropped-out sensor's task (and cleared it
// from its running mask), the sensor coming back gets a fresh task.
TEST(DiscoveryGroup, NewlyFoundRestartsASensorWhoseTaskWasStopped)
{
    uint8_t running = SENSOR_SHT31;    /* the DS18B20 task was stopped earlier */
    uint8_t found_now = SENSOR_SHT31 | SENSOR_DS18B20;

    uint8_t to_start = discovery_newly_found(running, found_now);

    LONGS_EQUAL(SENSOR_DS18B20, to_start);
}

/* ---- discovery_lost: what the discovery task stops ---- */

// Pins down: a sensor that has a task but stopped answering is the one to stop.
TEST(DiscoveryGroup, LostReturnsTheSensorThatStoppedAnswering)
{
    uint8_t running = SENSOR_SHT31 | SENSOR_DS18B20;
    uint8_t found_now = SENSOR_SHT31;    /* the DS18B20 did not answer this pass */

    uint8_t to_stop = discovery_lost(running, found_now);

    LONGS_EQUAL(SENSOR_DS18B20, to_stop);
}

// Pins down: nothing is stopped while every running sensor keeps answering.
TEST(DiscoveryGroup, LostIsEmptyWhileEverythingRunningStillAnswers)
{
    uint8_t running = SENSOR_SHT31 | SENSOR_DS18B20;
    uint8_t found_now = SENSOR_SHT31 | SENSOR_DS18B20;

    uint8_t to_stop = discovery_lost(running, found_now);

    LONGS_EQUAL(0, to_stop);
}

// Pins down: a sensor that is found but has no task yet is not "lost", it is new.
TEST(DiscoveryGroup, LostIgnoresASensorThatWasNeverRunning)
{
    uint8_t running = SENSOR_SHT31;
    uint8_t found_now = SENSOR_SHT31 | SENSOR_DS18B20;

    uint8_t to_stop = discovery_lost(running, found_now);

    LONGS_EQUAL(0, to_stop);
}

/* ---- boot behaviour ---- */

// Pins down: a node with nothing attached starts no tasks and keeps probing, and picks up a
// sensor that is plugged in later. This is one boot pass followed by one later pass.
TEST(DiscoveryGroup, BootWithNothingAttachedThenASensorAppearsLater)
{
    uint8_t running = 0;

    uint8_t found_at_boot = discovery_scan(&probes);
    uint8_t to_start_at_boot = discovery_newly_found(running, found_at_boot);

    fake_sht31_present = true;    /* someone plugs the SHT31 in */

    uint8_t found_later = discovery_scan(&probes);
    uint8_t to_start_later = discovery_newly_found(running, found_later);

    LONGS_EQUAL(0, to_start_at_boot);
    LONGS_EQUAL(SENSOR_SHT31, to_start_later);
}
