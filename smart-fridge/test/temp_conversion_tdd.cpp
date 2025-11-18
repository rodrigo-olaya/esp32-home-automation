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
}