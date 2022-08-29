#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <BobaBlox.h>
#include <Wire.h>
#include "paj7620.h"
#include "FastLED.h"

// Declarations of the speaker
Speaker piezo(33);

// Defining pin numbers for all the gestor sensor
#define S0 4
#define S1 16
#define S2 2
#define S3 15
#define sensorOut 0

// Stores frequency read by the color sensor
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

#define NUM_LEDS 7
#define LED_PIN 32
CRGB leds[NUM_LEDS];

#endif
