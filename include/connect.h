#ifndef __GESTURE_CAR_CONNECT_H__
#define __GESTURE_CAR_CONNECT_H__

#include <Arduino.h>

#include <WiFi.h>
#include <WebSocketsClient.h>

#include "header.h"

void connect_setup();
void connect_loop();
void send_message(String message);

#endif