#include "CppUTest/TestHarness.h"
#include "CppUTest/CommandLineTestRunner.h"

extern "C" {
    #include "../main/motors/handlers/calculate_steps.h"
}

TEST_GROUP(motorGroup)
{
};

TEST(motorGroup, calculateSteps)
{
   LONGS_EQUAL(calculate_steps(45, 1.8), 25);
}