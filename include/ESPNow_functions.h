#ifndef ESPNOW_FUNCTIONS_H
#define ESPNOW_FUNCTIONS_H

#include <Arduino.h>
#include <WiFi.h>
#include "ESPNowW.h"

void onRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);

#endif