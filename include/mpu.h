#ifndef __GESTURE_CAR_MPU_H__
#define __GESTURE_CAR_MPU_H__

#include <Arduino.h>

#include <math.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <SimpleKalmanFilter.h>

#define MPU6050_CORRECTION_TIMES 1000 // MPU6050校正次數
#define MPU6050_AZ_DEFAULT 10 // 台灣北部重力加速度

void mpu_correction();
void mpu_init(mpu6050_accel_range_t accelRange, mpu6050_gyro_range_t gyroRange, mpu6050_bandwidth_t bandwidth);
void mpu_loop();

#endif