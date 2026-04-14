#include <Arduino.h>
#include "uartProtocol.h"
#include "pins.h"
#include "servoController.h"
#include "irSensor.h"

UARTProtocol uart(Serial);

Servo catcherServo;
Servo armServo;

TCRT5000Analog irCather(pins.irSensor.catcher, 800);
TCRT5000Analog irDropPoint(pins.irSensor.dropPoint, 800);
TCRT5000Analog irShoot(pins.irSensor.shoot, 800);
TCRT5000Analog irSpeedMotorRight(pins.irSensor.speedMotorRight, 800);
TCRT5000Analog irSpeedMotorLeft(pins.irSensor.speedMotorLeft, 800);

int counter = 1;

void(* resetFunc) (void) = 0;

void uartCommand(byte cmd, byte id, byte *data, byte len) {
  if (cmd == uart.mapId.PING) {
    uart.send(uart.mapId.PONG, 0, NULL);
  } else if (cmd == uart.mapId.servo.catcher.ANGLE) {
    servo180(catcherServo, data[0]);
  } else if (cmd == uart.mapId.servo.arm.ANGLE) {
    servo180(armServo, data[0]);
  }
}




void setup() {
  Serial.begin(9600);
  uart.begin(uartCommand);
  servoAttach(catcherServo, pins.servo.catcher);
  servoAttach(armServo, pins.servo.arm);
}

void loop() {
  uart.update();
  if (irCather.isDetected()) {
    uart.send(uart.mapId.irSensor.CATCHER, 0, NULL);
  }
  
}
