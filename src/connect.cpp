#include "connect.h"

const uint8_t car_mac[6] = CAR_MAC;
const uint8_t controller_mac[6] = CONTROLLER_MAC;

esp_now_peer_info_t peerInfo;

/**
 * @brief ESP-NOW發送資料的回調函數
 * 
 * @param mac_addr MAC address of the peer
 * @param status Delivery status
 */
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

/**
 * @brief ESP-NOW接收資料的回調函數
 * 
 * @param mac MAC address of the sender
 * @param incomingData Received data
 * @param len Length of received data
 */
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    Serial.printf("Packet received from: %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    char *data = (char *)malloc(len + 1);
    if (data == NULL) {
        Serial.println("malloc failed");
        return;
    }
    memcpy(data, incomingData, len);
    data[len] = 0;
    Serial.printf("Received data: %s\n", data);
}

/**
 * @brief ESP-NOW的初始化
 * 
 */
void connect_setup() {
    WiFi.mode(WIFI_STA);
    Serial.println("Controller MAC address:" + WiFi.macAddress());
    Serial.printf("Car MAC address: %02X:%02X:%02X:%02X:%02X:%02X\n", car_mac[0], car_mac[1], car_mac[2], car_mac[3], car_mac[4], car_mac[5]);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        while(1);
    }

    esp_now_register_send_cb(OnDataSent);

    memcpy(peerInfo.peer_addr, car_mac, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }

    esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

/**
 * @brief 發送資料
 * 
 * @param message 要發送的資料
 */
void send_message(String message) {
    esp_err_t result = esp_now_send(car_mac, (uint8_t *)message.c_str(), message.length());
    if (result == ESP_OK) {
        Serial.println("Sent with success");
    } else {
        Serial.println("Error sending the data");
    }
}