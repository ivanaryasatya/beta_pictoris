#pragma once
#include <Arduino.h>
#include "motorDriver.h"

class MecanumDrive {
private:
    MotorDriver* leftDriver;
    MotorDriver* rightDriver;

public:
    /**
     * @brief Inisialisasi penggerak Mecanum
     * @param lDriver Pointer ke MotorDriver Kiri (Motor A = Depan, Motor B = Belakang)
     * @param rDriver Pointer ke MotorDriver Kanan (Motor A = Depan, Motor B = Belakang)
     */
    MecanumDrive(MotorDriver* lDriver, MotorDriver* rDriver) {
        leftDriver = lDriver;
        rightDriver = rDriver;
    }

    /**
     * @brief Menggerakkan roda Mecanum berdasarkan input sumbu
     * @param x Kecepatan strafe/geser Kiri (-128) hingga Kanan (127)
     * @param y Kecepatan Maju (127) hingga Mundur (-128) 
     * @param turn Kecepatan rotasi Kiri (-128) hingga Kanan (127)
     */
    void drive(int x, int y, int turn) {
        // Filter Deadzone joystick (menghilangkan noise saat stik ditekan tengah)
        if (abs(x) < 10) x = 0;
        if (abs(y) < 10) y = 0;
        if (abs(turn) < 10) turn = 0;

        // Kinematika Mecanum Wheel
        // FL = Front Left, FR = Front Right, BL = Back Left, BR = Back Right
        int fl = y + x + turn;
        int fr = y - x - turn;
        int bl = y - x + turn;
        int br = y + x - turn;

        // Cari nilai absolut maksimum untuk normalisasi
        int maxVal = max(abs(fl), max(abs(fr), max(abs(bl), abs(br))));

        // Jika hasil penjumlahan melebihi kapasitas max joystick (127),
        // kita lakukan normalisasi rasio agar arah gerakan tetap proporsional.
        if (maxVal > 127) {
            fl = fl * 127 / maxVal;
            fr = fr * 127 / maxVal;
            bl = bl * 127 / maxVal;
            br = br * 127 / maxVal;
        }

        // Mapping rentang joystick gamepad (-127 s/d 127) ke rentang PWM Motor (-255 s/d 255)
        fl = map(fl, -127, 127, -255, 255);
        fr = map(fr, -127, 127, -255, 255);
        bl = map(bl, -127, 127, -255, 255);
        br = map(br, -127, 127, -255, 255);

        // Eksekusi PWM ke masing-masing motor
        // Asumsi: Motor A pada driver adalah Roda Depan, Motor B adalah Roda Belakang
        leftDriver->setMotorA(fl);
        leftDriver->setMotorB(bl);
        rightDriver->setMotorA(fr);
        rightDriver->setMotorB(br);
    }

    /**
     * @brief Menghentikan seluruh pergerakan robot
     */
    void stop() {
        leftDriver->stop();
        rightDriver->stop();
    }
};
