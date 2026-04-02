#pragma once

#if defined(ESP32)

#include <Arduino.h>
#include <Ps3Controller.h>

/**
 * Ini adalah header file untuk menghandle koneksi, input dan fitur
 * dari stik PS3 (Sixaxis DualShock 3) menggunakan library jvpernis/PS3 Controller.
 */

// Struct untuk menyimpan semua status gamepad PS3
struct GamepadState {
    // === Tombol Digital (true = ditekan, false = dilepas) ===
    bool cross, square, triangle, circle;
    bool up, right, down, left;
    bool l1, l2, r1, r2, l3, r3;
    bool select, start, ps;

    // === Tombol Analog/Pressure Sensitive (0 - 255) ===
    // Semakin keras tombol ditekan, semakin besar nilainya
    uint8_t analog_cross, analog_square, analog_triangle, analog_circle;
    uint8_t analog_up, analog_right, analog_down, analog_left;
    uint8_t analog_l1, analog_l2, analog_r1, analog_r2;

    // === Joystick Analog (-128 hingga 127) ===
    int8_t stick_lx, stick_ly;  // Left stick (X, Y)
    int8_t stick_rx, stick_ry;  // Right stick (X, Y)

    // === Sensor Gerak (Accelerometer & Gyroscope) ===
    int16_t accel_x, accel_y, accel_z;
    int16_t gyro_z;

    // === Status Baterai ===
    String battery_status;
};

// Variabel global state yang bisa diakses di main.cpp
extern GamepadState gamepad_state; 
GamepadState gamepad_state;

// Fungsi callback saat PS3 Controller berhasil terkoneksi
inline void onPs3Connect() {
    Serial.println("\n[SISTEM] PS3 Controller Terkoneksi!");
    
    // Set LED Player 1
    Ps3.setPlayer(1);
    
    // Memberikan feedback getar selama 500ms
    // Ps3.setRumble(intensity for small motor, intensity for large motor)
    Ps3.setRumble(100, 100); 
    delay(500);
    Ps3.setRumble(0, 0); // Matikan getaran
}

// Fungsi callback yang dipanggil otomatis saat ada data/event baru dari stik PS3
inline void onPs3Event() {
    // == 1. Membaca Tombol Digital ==
    gamepad_state.cross    = Ps3.data.button.cross;
    gamepad_state.square   = Ps3.data.button.square;
    gamepad_state.triangle = Ps3.data.button.triangle;
    gamepad_state.circle   = Ps3.data.button.circle;
    gamepad_state.up       = Ps3.data.button.up;
    gamepad_state.right    = Ps3.data.button.right;
    gamepad_state.down     = Ps3.data.button.down;
    gamepad_state.left     = Ps3.data.button.left;
    gamepad_state.l1       = Ps3.data.button.l1;
    gamepad_state.l2       = Ps3.data.button.l2;
    gamepad_state.r1       = Ps3.data.button.r1;
    gamepad_state.r2       = Ps3.data.button.r2;
    gamepad_state.l3       = Ps3.data.button.l3;
    gamepad_state.r3       = Ps3.data.button.r3;
    gamepad_state.select   = Ps3.data.button.select;
    gamepad_state.start    = Ps3.data.button.start;
    gamepad_state.ps       = Ps3.data.button.ps;

    // == 2. Membaca Nilai Analog dari Tombol (Pressure Sensitive) ==
    gamepad_state.analog_cross    = Ps3.data.analog.button.cross;
    gamepad_state.analog_square   = Ps3.data.analog.button.square;
    gamepad_state.analog_triangle = Ps3.data.analog.button.triangle;
    gamepad_state.analog_circle   = Ps3.data.analog.button.circle;
    gamepad_state.analog_up       = Ps3.data.analog.button.up;
    gamepad_state.analog_right    = Ps3.data.analog.button.right;
    gamepad_state.analog_down     = Ps3.data.analog.button.down;
    gamepad_state.analog_left     = Ps3.data.analog.button.left;
    gamepad_state.analog_l1       = Ps3.data.analog.button.l1;
    gamepad_state.analog_l2       = Ps3.data.analog.button.l2;
    gamepad_state.analog_r1       = Ps3.data.analog.button.r1;
    gamepad_state.analog_r2       = Ps3.data.analog.button.r2;

    // == 3. Membaca Joystick Analog ==
    gamepad_state.stick_lx = Ps3.data.analog.stick.lx;
    gamepad_state.stick_ly = Ps3.data.analog.stick.ly;
    gamepad_state.stick_rx = Ps3.data.analog.stick.rx;
    gamepad_state.stick_ry = Ps3.data.analog.stick.ry;

    // == 4. Membaca Sensor Gerak (Accelerometer & Gyroscope) ==
    gamepad_state.accel_x = Ps3.data.sensor.accelerometer.x;
    gamepad_state.accel_y = Ps3.data.sensor.accelerometer.y;
    gamepad_state.accel_z = Ps3.data.sensor.accelerometer.z;
    gamepad_state.gyro_z  = Ps3.data.sensor.gyroscope.z;

    // == 5. Memeriksa Status Baterai (contoh mengambil state pas nahan tombol PS) ==
    int batStatus = Ps3.data.status.battery;
    if (batStatus == ps3_status_battery_charging) {
        gamepad_state.battery_status = "Charging";
    } else if (batStatus == ps3_status_battery_high) {
        gamepad_state.battery_status = "High";
    } else if (batStatus == ps3_status_battery_full) {
        gamepad_state.battery_status = "Full";
    } else if (batStatus == ps3_status_battery_low) {
        gamepad_state.battery_status = "Low";
    } else if (batStatus == ps3_status_battery_dying) {
        gamepad_state.battery_status = "Dying";
    } else if (batStatus == ps3_status_battery_shutdown) {
        gamepad_state.battery_status = "Shutdown";
    } else {
        gamepad_state.battery_status = "Unknown";
    }
}

// === FUNGSI UTAMA UNTUK DIPANGGIL DI MAIN.CPP ===

/**
 * Inisialisasi PS3 Controller
 * Panggil fungsi ini di dalam setup() main.cpp.
 * @param mac_address MAC Address Bluetooth dari ESP32 Anda.
 */
inline void initGamepad(const char* mac_address) {
    // Melekatkan callback untuk menerima event
    Ps3.attach(onPs3Event);
    // Melekatkan callback untuk koneksi berhasil
    Ps3.attachOnConnect(onPs3Connect);
    
    // Mulai listening koneksi PS3 dengan MAC address ini
    Ps3.begin(mac_address);
    Serial.print("\n[INFO] Gamepad Controller Diinisialisasi.");
    Serial.print(" Menunggu koneksi PS3 di MAC: ");
    Serial.println(mac_address);
}

/**
 * Contoh Print Data Gamepad untuk Debugging
 * Bisa dipanggil secara berkala di loop() jika ingin melihat perubahan datanya.
 */
inline void printGamepadDebug() {
    if (!Ps3.isConnected()) return;

    // Contoh menampilkan beberapa nilai stik ke serial monitor
    Serial.printf("LX: %d, LY: %d | RX: %d, RY: %d | L2 Analog: %d | R2 Analog: %d | Bat: %s\n",
        gamepad_state.stick_lx,
        gamepad_state.stick_ly,
        gamepad_state.stick_rx,
        gamepad_state.stick_ry,
        gamepad_state.analog_l2,
        gamepad_state.analog_r2,
        gamepad_state.battery_status.c_str()
    );

    // Contoh cek apakah R1 ditekan
    if (gamepad_state.r1) {
        Serial.println(">> Tombol R1 Sedang Ditekan!");
    }

     // Contoh getar saat tekan segitiga
    if (Ps3.event.button_down.triangle){
         Ps3.setRumble(100, 100);    
    }
    if (Ps3.event.button_up.triangle){
         Ps3.setRumble(0, 0);    
    }
}

#endif // ESP32
