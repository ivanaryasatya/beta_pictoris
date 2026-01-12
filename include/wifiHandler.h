#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include "secretData.h"

String WIFI_SSID = secretData.getWifiSSID();
String WIFI_PASSWORD = secretData.getWifiPassword();

void connect() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println("\nWiFi terhubung.");
}

void disconnect() {
  WiFi.disconnect(true);
  Serial.println("WiFi terputus.");
}