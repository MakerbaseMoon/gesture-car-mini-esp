#include <Arduino.h>

#include "mpu.h"
#include "connect.h"

void setup() {
    Serial.begin(115200);
    while(!Serial);

    connect_setup();
    mpu_init(MPU6050_RANGE_8_G, MPU6050_RANGE_500_DEG, MPU6050_BAND_5_HZ);
}

void loop() {
    mpu_loop();
    connect_loop();
    delay(10);
}