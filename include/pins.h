#pragma once
#include <Arduino.h>
using cbyte = const byte;

struct Pins {

    struct L298N {
        struct Motors {
            cbyte IN1 = 25;
            cbyte IN2 = 26;
            cbyte IN3 = 27;
            cbyte IN4 = 14;
            cbyte ENA = 33;
            cbyte ENB = 32;
        } motors;

    } l298n;

};

extern Pins pins;