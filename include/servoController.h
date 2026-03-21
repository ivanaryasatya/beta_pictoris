#pragma once

#include <Arduino.h>
#include <Servo.h>

inline void servoAttach(Servo &s, byte pin) {
  s.attach(pin);
}

inline void servo180(Servo &s, byte angle) {
  if (angle > 180) angle = 180;
  s.write(angle);
}

inline void servo360Stop(Servo &s) {
  s.write(90);
}

inline void servo360Left(Servo &s, byte speed = 30) {
  s.write(90 - speed);
}
inline void servo360Right(Servo &s, byte speed = 30) {
  s.write(90 + speed);
}