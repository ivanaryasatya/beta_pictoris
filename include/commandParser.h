#pragma once
#include <Arduino.h>

inline String getRawInput() {
  if (Serial.available() > 0) {
    String pesan = Serial.readStringUntil('\n');
    pesan.trim();
    return pesan;
  }
}

