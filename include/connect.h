#ifndef __GESTURE_CAR_CONNECT_H__
#define __GESTURE_CAR_CONNECT_H__

#include <Arduino.h>

#include <WiFi.h>
#include <esp_now.h>

#include "header.h"

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
void connect_setup();
void send_message(String message);

#endif