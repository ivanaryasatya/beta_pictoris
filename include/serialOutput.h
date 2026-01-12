#pragma once
#include <Arduino.h>
#include "commandParser.h"
class SerialOutput {
    private:
        int currentOutputNum = 1;
    public:
        inline void write(const byte outputNum, const String &message, const bool newLine = true) {
            if (currentOutputNum == outputNum) {
                if (newLine) {
                    Serial.println(message);
                } else {
                    Serial.print(message);
                }
            } else if (currentOutputNum == -1) {
                if (newLine) {
                    Serial.println(message);
                } else {
                    Serial.print(message);
                }
            }
        }
        inline void setCurrentOutputNum(const byte value) {
            currentOutputNum = value;
        }
};