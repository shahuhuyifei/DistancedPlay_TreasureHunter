#ifndef GESTURE_FUNCTIONS_H
#define GESTURE_FUNCTIONS_H

#include <Arduino.h>
#include "paj7620.h"

void initPaj720();
bool readGesture(uint8_t GES);

#endif