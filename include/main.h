#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <Wire.h>
#include "gesture_functions.h"
#include "FastLED.h"
#include <BobaBlox.h>
#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         25          // Configurable, see typical pin layout above
#define SS_PIN          26  

MFRC522 mfrc522(SS_PIN, RST_PIN);

// Define the servo that rotate the gear
Servo gearServo;
// Define the pin for gearServo
#define GEARSERVO_PIN 21

// Define a speaker called piezo on a pin
Speaker piezo(32);

// Define pin numbers for color sensor
#define S0 27
#define S1 33
#define S2 32
#define S3 14
#define sensorOut 15

// defining gesture game array size
#define NUM_GESTURES 10
int gestureGameTime = 0;

// Stores frequency read by the color sensor
int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

// Defining number of leds and pin for the led strip
#define NUM_LEDS 8
#define LED_PIN 16
// Declarations of the led strip
CRGB leds[NUM_LEDS];

// Stores RGB colors
int colorRed[3] = {255, 0, 0};
int colorBlue[3] = {0, 0, 255};
int colorYellow[3] = {255, 255, 0};
int colorGreen[3] = {0, 255, 0};

#endif
