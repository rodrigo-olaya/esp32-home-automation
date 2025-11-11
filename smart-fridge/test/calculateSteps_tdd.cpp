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
   DOUBLES_EQUAL(calculate_steps(25), 35.0, 0.01);
}