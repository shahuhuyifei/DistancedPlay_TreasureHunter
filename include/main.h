#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <Wire.h>
#include "gesture_functions.h"
#include <BobaBlox.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>
#include "ESPNow_functions.h"

// Define pins for RFID reader
#define RST_PIN         25   
#define SS_PIN          26  

// Declaration of the RFID reader
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Define a speaker called piezo on a pin
Speaker piezo(32);

// defining gesture game array size
#define NUM_GESTURES 10
int gestureGameTime = 0;

// Defining number of leds and pin for the neopixel
#define NUMPIXELS  8
#define LED_PIN   16

// Declaration of the neopixel
Adafruit_NeoPixel pixels(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Store RGB colors
uint32_t red = pixels.Color(255, 0, 0);
uint32_t blue = pixels.Color(0, 0, 255);
uint32_t yellow = pixels.Color(55, 255, 0);
uint32_t green = pixels.Color(0, 255, 0);
uint32_t black = pixels.Color(0, 0, 0);

// Store RFID card IDs
#define NUM_CARDS 6
String playerACards[NUM_CARDS] = {"BE 5D 4C 7F", "2E 68 07 90", "8E 78 07 90", "4E D0 06 90", "1E 54 07 90", "4E 4B 4C 7F"};
String playerBCards[NUM_CARDS] = {"AE 69 07 90", "DE 0B 4C 7F", "1E 80 07 90", "9E 6B 4C 7F", "9E 0B 4C 7F", "8E 5A 07 90"};

// Store MAC addresses
uint8_t playerA_mac[] = {0x3C, 0x61, 0x05, 0x4B, 0x05, 0x6C};
uint8_t playerB_mac[] = {0xE8, 0x31, 0xCD, 0x63, 0x5F, 0xD0};

uint8_t treasure_card = 0;
uint8_t gestureGame_status = 0;
uint8_t gestureGame_result = 0;
#endif
