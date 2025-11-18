#include "handle_temperature.h"

double convert_to_farenheit(double celsius) {
    return celsius * 9/5 + 32;
}

int round_temperature(double farenheit){
    return round(farenheit);
}