#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "MX28.h"

void printDouble( double val, unsigned int precision);
void toggleLEDXTimes(MX28 exoMotor, short boardPin, short tglnr, int tglSpeed);
bool isWithinRange(float value, float target, float percentage);
//bool isWithinRange(int32_t value, int32_t target, float percentage);

#endif