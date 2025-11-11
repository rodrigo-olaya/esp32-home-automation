#include "calculate_steps.h"

int calculate_steps(double angle, float resolution) {
    int steps =  angle / resolution;
    return steps;
}