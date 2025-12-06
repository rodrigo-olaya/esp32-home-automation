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
   LONGS_EQUAL(calculate_angle(4, 0), 90);
}