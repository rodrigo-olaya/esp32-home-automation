#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

extern "C" {
    #include "../main/motors/handlers/calculate_angle.h"
}

TEST_GROUP(angleGroup)
{
};

TEST(angleGroup, calculateAngle)
{
   DOUBLES_EQUAL(calculate_angle(4, 0), 90.0, 0.01);
   DOUBLES_EQUAL(calculate_angle(0, 0), 0.0, 0.01);
   DOUBLES_EQUAL(calculate_angle(8, 0), 180.0, 0.01);
}