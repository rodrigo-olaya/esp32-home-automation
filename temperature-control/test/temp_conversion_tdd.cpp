#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

extern "C" {
    #include "../main/data/handle_temperature.h"
}

TEST_GROUP(TempConversionGroup)
{
};

TEST(TempConversionGroup, ConvertToFarenheit)
{
   DOUBLES_EQUAL(convert_to_farenheit(20), 68.0, 0.01);
};

TEST(TempConversionGroup, RoundTemperature)
{
    LONGS_EQUAL(round_temperature(50.0345), 50);
    LONGS_EQUAL(round_temperature(40.5), 41);
    LONGS_EQUAL(round_temperature(55.555), 56);
}

TEST(TempConversionGroup, RawOneWireToTemperature)
{
    DOUBLES_EQUAL(25.0, raw_onewire_to_temp(0x01, 0x90), 0.01);
    DOUBLES_EQUAL(0.0, raw_onewire_to_temp(0x00, 0x00), 0.01);
    DOUBLES_EQUAL(25.0625, raw_onewire_to_temp(0x01, 0x91), 0.01);
    DOUBLES_EQUAL(-10.0, raw_onewire_to_temp(0xFF, 0x60), 0.01);
    DOUBLES_EQUAL(-0.5, raw_onewire_to_temp(0xFF, 0xF8), 0.01);
    DOUBLES_EQUAL(125.0, raw_onewire_to_temp(0x07, 0xD0), 0.01);
    DOUBLES_EQUAL(-55.0, raw_onewire_to_temp(0xFC, 0x90), 0.01);    
}