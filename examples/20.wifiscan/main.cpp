#include <Arduino.h>
#include <WiFi.h>

void setup() {
    Serial.begin(115200);
    while(!Serial);
}

void loop() {
    Serial.println("Scanning available networks...");
    int numNetworks = WiFi.scanNetworks();
    Serial.println("Networks found:");
    for (int i = 0; i < numNetworks; i++) {
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.println(" dBm)");
    }
    delay(5000);
}
