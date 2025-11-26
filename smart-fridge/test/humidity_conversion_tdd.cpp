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
    DOUBLES_EQUAL(57.7, bytes_to_humidity(147, 194), 0.01);
    DOUBLES_EQUAL(0.0, bytes_to_humidity(0, 0), 0.01);
    DOUBLES_EQUAL(50.0, bytes_to_humidity(128, 0), 0.01);
    DOUBLES_EQUAL(100.0, bytes_to_humidity(255,255), 0.01);
    DOUBLES_EQUAL(33.3, bytes_to_humidity(85,51), 0.01);
    DOUBLES_EQUAL(80.0, bytes_to_humidity(204,204), 0.01);
};