#pragma once
#include <Arduino.h>

/**
 * PENGOPTIMALAN MEMORI (PROGMEM SEAMLESS)
 * 
 * Di C++ (khususnya Arduino core untuk ESP32), menggunakan `static constexpr`
 * akan secara otomatis memastikan string literal diletakkan di dalam Flash Memory 
 * (.rodata segment, setara PROGMEM) dan tidak akan menggunakan RAM sama sekali 
 * untuk pointer variabel global saat runtime.
 * 
 * Kita cukup membuat objek kosong "logMes" sehingga kode program `.cpp` Anda 
 * yang lain (yang sudah pakai `logMes.pesan`) tetap bisa bekerja (backward compatibility)
 * tanpa perlu mengubah apapun.
 */
struct LogMessage {
    static constexpr const char* wifiHasConnected = "wifi is already connected";
    static constexpr const char* wifiConnecting = "connecting to wifi";
    static constexpr const char* wifiConnected = "wifi connected";
    static constexpr const char* wifiNotConnected = "wifi not connected";
    static constexpr const char* wifiConnectionFailed = "wifi connection failed";
    static constexpr const char* retryingConnection = "retrying connection...";
    static constexpr const char* wifiStartingScan = "starting wifi scan...";
    static constexpr const char* wifiNoNetworksFound = "no wifi networks found";
    static constexpr const char* wifiNetworksFound = "wifi networks found";
    static constexpr const char* credentialsUpdated = "wifi credentials updated";
    static constexpr const char* newSSID = "new SSID: ";
    static constexpr const char* newPassword = "new Password: ";
    static constexpr const char* accessPointStarted = "access point started";
    static constexpr const char* ssidAp = "SSID AP: ";
    static constexpr const char* ipAp = "IP AP: ";
    static constexpr const char* accessPointFailed = "failed to start access point";
    static constexpr const char* accessPointSuccessfullyConfigured = "access point IP configuration successfully changed";
    static constexpr const char* accessPointConfigurationFailed = "failed to change access point IP configuration";
    static constexpr const char* accessPointStopped = "access point stopped";
    static constexpr const char* invalidCommandTarget = "invalid command target";
    static constexpr const char* invalidCommand = "invalid command";
    static constexpr const char* esp32Restarting = "ESP32 is restarting...";
    static constexpr const char* commandValueLessThanExpected = "command value count is less than ";
    static constexpr const char* wifiLocalIP = "wifi local IP: ";
    static constexpr const char* eepromSaveSuccess = "successfully saved to EEPROM";
    static constexpr const char* eepromSaveFailed = "failed to save to EEPROM";
    static constexpr const char* esp32WillRestartIn = "ESP32 will restart in ";
    static constexpr const char* seconds = " seconds";
    static constexpr const char* ON = "ON";
    static constexpr const char* OFF = "OFF";
    static constexpr const char* logStatus = "Log status: ";
};


constexpr LogMessage logMes;