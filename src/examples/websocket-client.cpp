#include <Arduino.h>

#include <WiFi.h>
#include <WebSocketsClient.h>

#include "header.h"

int count = 0;

WebSocketsClient webSocket;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
	switch(type) {
		case WStype_DISCONNECTED:
			Serial.printf("[ws] Disconnected!\n");
			break;
		case WStype_CONNECTED:
			Serial.printf("[ws] Connected to url: %s\n", payload);
			break;
		case WStype_TEXT:
			Serial.printf("[ws] Get text: %s\n", payload);
			break;
        default:
			break;
	}
}

void setup() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(CAR_SSID, CAR_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println("Connected to the WiFi network");
    Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());

    webSocket.begin("192.168.4.1", 80, "/ws");
    webSocket.onEvent(webSocketEvent);
}

void loop() {
	webSocket.loop();
    webSocket.sendTXT("Hello world: " + String(count++) + "!");
    delay(1000);
}
