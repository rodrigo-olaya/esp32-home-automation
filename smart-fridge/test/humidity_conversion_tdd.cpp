#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

extern "C" {
    #include "../main/data/handle_humidity.h"
}

TEST_GROUP(HumidityConversionGroup)
{
};

TEST(HumidityConversionGroup, BytesToHumidity)
{
   DOUBLES_EQUAL(bytes_to_humidity(147, 194), 57.7, 0.01);
};