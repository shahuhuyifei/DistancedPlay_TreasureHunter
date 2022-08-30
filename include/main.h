#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <Wire.h>
#include "gesture_functions.h"
#include "FastLED.h"
#include <BobaBlox.h>

Speaker piezo(19);

// Defining pin numbers for all the gesture sensor
#define S0 4
#define S1 16
#define S2 2
#define S3 15
#define sensorOut 0
// defining gesture game array size
#define NUM_GESTURES 10
int gestureGameTime = 0;

// Stores frequency read by the color sensor
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Defining number of leds and pin for the led strip
#define NUM_LEDS 7
#define LED_PIN 32
// Declarations of the led strip
CRGB leds[NUM_LEDS];

// Stores RGB colors
int colorRed[3] = {255, 0, 0};
int colorBlue[3] = {0, 0, 255};
int colorYellow[3] = {255, 255, 0};
int colorGreen[3] = {0, 255, 0};

#endif
