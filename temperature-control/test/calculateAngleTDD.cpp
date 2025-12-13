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
   DOUBLES_EQUAL(90.0, calculate_angle(4, 0), 0.1);
   DOUBLES_EQUAL(0.0, calculate_angle(0, 0), 0.1);
   DOUBLES_EQUAL(180.0, calculate_angle(8, 0), 0.1);
   DOUBLES_EQUAL(270.0, calculate_angle(12, 0), 0.1);
   DOUBLES_EQUAL(359.91, calculate_angle(15, 255), 0.1);
   DOUBLES_EQUAL(154.7, calculate_angle(6, 224), 0.1);
   DOUBLES_EQUAL(10.0, calculate_angle(0, 114), 0.1);
}