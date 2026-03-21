#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

class GyroSensor {
private:
    Adafruit_MPU6050 mpu;
    bool isInitialized = false;

    // Variabel untuk menyimpan kalibrasi (offset)
    float accelXOffset = 0, accelYOffset = 0, accelZOffset = 0;
    float gyroXOffset = 0, gyroYOffset = 0, gyroZOffset = 0;

public:
    GyroSensor() {
    }

    // Inisialisasi MPU6050
    bool begin() {
        Serial.println("Memulai inisialisasi MPU6050...");
        
        // Coba inisialisasi
        if (!mpu.begin()) {
            Serial.println("Gagal menemukan chip MPU6050! Pastikan kabel VCC, GND, SCL, dan SDA terhubung dengan benar.");
            isInitialized = false;
            return false;
        }

        Serial.println("MPU6050 Ditemukan!");

        // Setel rentang akselerometer (Default: 8G)
        mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
        // Setel rentang giroskop (Default: 500 deg/s)
        mpu.setGyroRange(MPU6050_RANGE_500_DEG);
        // Setel filter bandwidth (Default: 21 Hz)
        mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

        isInitialized = true;
        delay(100);
        return true;
    }

    // Mengecek apakah sensor sudah siap digunakan
    bool isReady() {
        return isInitialized;
    }

    // Mendapatkan Acceleration (Gaya Gravitasi/Percepatan) dalam m/s^2 X
    float getAccelX() {
        if (!isInitialized) return 0.0;
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        return a.acceleration.x - accelXOffset;
    }

    // Mendapatkan Acceleration dalam m/s^2 Y
    float getAccelY() {
        if (!isInitialized) return 0.0;
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        return a.acceleration.y - accelYOffset;
    }

    // Mendapatkan Acceleration dalam m/s^2 Z
    float getAccelZ() {
        if (!isInitialized) return 0.0;
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        return a.acceleration.z - accelZOffset;
    }

    // Mendapatkan rotasi Gyro (Kecepatan Sudut) dalam rad/s X
    float getGyroX() {
        if (!isInitialized) return 0.0;
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        return g.gyro.x - gyroXOffset;
    }

    // Mendapatkan rotasi Gyro dalam rad/s Y
    float getGyroY() {
        if (!isInitialized) return 0.0;
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        return g.gyro.y - gyroYOffset;
    }

    // Mendapatkan rotasi Gyro dalam rad/s Z
    float getGyroZ() {
        if (!isInitialized) return 0.0;
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        return g.gyro.z - gyroZOffset;
    }

    // Mendapatkan Suhu Chip MPU6050 (Celsius)
    float getTemperature() {
        if (!isInitialized) return 0.0;
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        return temp.temperature;
    }

    // Print semua data sekaligus ke Serial Monitor (Berguna untuk debug)
    void printData() {
        if (!isInitialized) {
            Serial.println("MPU6050 belum diinisialisasi.");
            return;
        }

        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        Serial.print("AccelX: "); Serial.print(a.acceleration.x - accelXOffset); Serial.print(" m/s^2, ");
        Serial.print("AccelY: "); Serial.print(a.acceleration.y - accelYOffset); Serial.print(" m/s^2, ");
        Serial.print("AccelZ: "); Serial.print(a.acceleration.z - accelZOffset); Serial.println(" m/s^2");

        Serial.print("GyroX: "); Serial.print(g.gyro.x - gyroXOffset); Serial.print(" rad/s, ");
        Serial.print("GyroY: "); Serial.print(g.gyro.y - gyroYOffset); Serial.print(" rad/s, ");
        Serial.print("GyroZ: "); Serial.print(g.gyro.z - gyroZOffset); Serial.println(" rad/s");

        Serial.print("Suhu: "); Serial.print(temp.temperature); Serial.println(" degC\n");
    }

    // Auto-Kalibrasi Posisi Nol (Harus dilakukan saat sensor diam alias ditaruh di tempat datar)
    void calibrate(unsigned int samples = 500) {
        if (!isInitialized) {
            Serial.println("Gagal kalibrasi: MPU6050 belum diinisialisasi.");
            return;
        }

        Serial.println("Memulai kalibrasi MPU6050... (Tolong jangan gerakkan sensor!)");
        float sumAx = 0, sumAy = 0, sumAz = 0;
        float sumGx = 0, sumGy = 0, sumGz = 0;

        sensors_event_t a, g, temp;

        for (int i = 0; i < samples; i++) {
            mpu.getEvent(&a, &g, &temp);
            sumAx += a.acceleration.x;
            sumAy += a.acceleration.y;
            // Kita biarkan gravitasi Z untuk bernilai gaya ~9.8 m/s^2
            sumAz += (a.acceleration.z - 9.80665); 
            
            sumGx += g.gyro.x;
            sumGy += g.gyro.y;
            sumGz += g.gyro.z;

            delay(5); // Ambil jeda 5ms tiap sampel ~2.5 detik
        }

        accelXOffset = sumAx / samples;
        accelYOffset = sumAy / samples;
        accelZOffset = sumAz / samples;

        gyroXOffset = sumGx / samples;
        gyroYOffset = sumGy / samples;
        gyroZOffset = sumGz / samples;

        Serial.println("Kalibrasi selesai!");
        Serial.println("Offset yang didapat:");
        Serial.print("Accel (X, Y, Z): "); 
        Serial.print(accelXOffset); Serial.print(", ");
        Serial.print(accelYOffset); Serial.print(", ");
        Serial.println(accelZOffset);

        Serial.print("Gyro (X, Y, Z): ");
        Serial.print(gyroXOffset); Serial.print(", ");
        Serial.print(gyroYOffset); Serial.print(", ");
        Serial.println(gyroZOffset);
    }
    
    // Konfigurasi Range Secara Manual: Accelerometer
    void setAccelRange(mpu6050_accel_range_t range) {
        if(isInitialized) mpu.setAccelerometerRange(range);
    }

    // Konfigurasi Range Secara Manual: Gyroscope
    void setGyroRange(mpu6050_gyro_range_t range) {
        if(isInitialized) mpu.setGyroRange(range);
    }
};

extern GyroSensor gyro;
