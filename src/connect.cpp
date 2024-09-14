#include "connect.h"

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
		// case WStype_BIN:
		// case WStype_ERROR:			
		// case WStype_FRAGMENT_TEXT_START:
		// case WStype_FRAGMENT_BIN_START:
		// case WStype_FRAGMENT:
		// case WStype_FRAGMENT_FIN:
        default:
			break;
	}
}

void connect_setup() {
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

void connect_loop() {
	webSocket.loop();
}

void send_message(String message) {
    webSocket.sendTXT(message);
}