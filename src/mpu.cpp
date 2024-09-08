#include "mpu.h"

Adafruit_MPU6050 mpu;

float a_x_offset = 0.0, a_y_offset = 0.0, a_z_offset = 0.0;
float g_x_offset = 0.0, g_y_offset = 0.0, g_z_offset = 0.0;
double yaw_offset = 0.0;

float accPitch = 0;
float accRoll = 0;
double accYaw = 0;

float kalPitch = 0;
float kalRoll = 0;
double kalYaw = 0;

SimpleKalmanFilter kalmanPitch(0.1, 0.1, 0.01);
SimpleKalmanFilter kalmanRoll(0.1, 0.1, 0.01);
SimpleKalmanFilter kalmanYaw(0.1, 0.1, 0.01);

unsigned long lastTime = 0;

void mpu_correction() {
    double total_a_x = 0, total_a_y = 0, total_a_z = 0;
    double total_g_x = 0, total_g_y = 0, total_g_z = 0;

    for (int i = 0; i < MPU6050_CORRECTION_TIMES; i++) {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        total_a_x += a.acceleration.x;
        total_a_y += a.acceleration.y;
        total_a_z += a.acceleration.z;

        total_g_x += g.gyro.x;
        total_g_y += g.gyro.y;
        total_g_z += g.gyro.z;
        delay(10);
    }

    a_x_offset = total_a_x / MPU6050_CORRECTION_TIMES;
    a_y_offset = total_a_y / MPU6050_CORRECTION_TIMES;
    a_z_offset = total_a_z / MPU6050_CORRECTION_TIMES - MPU6050_AZ_DEFAULT;
    
    g_x_offset = total_g_x / MPU6050_CORRECTION_TIMES;
    g_y_offset = total_g_y / MPU6050_CORRECTION_TIMES;
    g_z_offset = total_g_z / MPU6050_CORRECTION_TIMES;
    Serial.printf("a_x_offset: %f, a_y_offset: %f, a_z_offset: %f\n", a_x_offset, a_y_offset, a_z_offset);
    Serial.printf("g_x_offset: %f, g_y_offset: %f, g_z_offset: %f\n", g_x_offset, g_y_offset, g_z_offset);
}


void mpu_init(mpu6050_accel_range_t accelRange, mpu6050_gyro_range_t gyroRange, mpu6050_bandwidth_t bandwidth) {
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        return;
    }
    Serial.println("MPU6050 Found!");

    mpu.setAccelerometerRange(accelRange);
    Serial.print("Accelerometer range set to: ");
    switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
        Serial.println("+-2G");
        break;
    case MPU6050_RANGE_4_G:
        Serial.println("+-4G");
        break;
    case MPU6050_RANGE_8_G:
        Serial.println("+-8G");
        break;
    case MPU6050_RANGE_16_G:
        Serial.println("+-16G");
        break;
    }
    mpu.setGyroRange(gyroRange);
    Serial.print("Gyro range set to: ");
    switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
        Serial.println("+- 250 deg/s");
        break;
    case MPU6050_RANGE_500_DEG:
        Serial.println("+- 500 deg/s");
        break;
    case MPU6050_RANGE_1000_DEG:
        Serial.println("+- 1000 deg/s");
        break;
    case MPU6050_RANGE_2000_DEG:
        Serial.println("+- 2000 deg/s");
        break;
    }

    mpu.setFilterBandwidth(bandwidth);
    Serial.print("Filter bandwidth set to: ");
    switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
        Serial.println("260 Hz");
        break;
    case MPU6050_BAND_184_HZ:
        Serial.println("184 Hz");
        break;
    case MPU6050_BAND_94_HZ:
        Serial.println("94 Hz");
        break;
    case MPU6050_BAND_44_HZ:
        Serial.println("44 Hz");
        break;
    case MPU6050_BAND_21_HZ:
        Serial.println("21 Hz");
        break;
    case MPU6050_BAND_10_HZ:
        Serial.println("10 Hz");
        break;
    case MPU6050_BAND_5_HZ:
        Serial.println("5 Hz");
        break;
    }

    Serial.println("");
    mpu_correction();
    delay(100);
    lastTime = millis();
}

void mpu_loop() {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    unsigned long currentTime = millis();

    float accX = a.acceleration.x - a_x_offset;
    float accY = a.acceleration.y - a_y_offset;
    float accZ = a.acceleration.z - a_z_offset;

    float gyroX = g.gyro.x - g_x_offset;
    float gyroY = g.gyro.y - g_y_offset;
    double gyroZ = g.gyro.z - g_z_offset;

    accPitch = atan2(-accY, sqrt(accX * accX + accZ * accZ)) * 180 / PI;
    accRoll = atan2(accX, accZ) * 180.0 / M_PI;
    accYaw += gyroZ * (double(currentTime - lastTime) / 1000.0);

    accPitch = kalmanPitch.updateEstimate(accPitch);
    accRoll = kalmanRoll.updateEstimate(accRoll);
    accYaw = kalmanYaw.updateEstimate(accYaw);

    Serial.printf("%f, %f, %f\n", accPitch, accRoll, accYaw);
    lastTime = currentTime;
}