#include <Arduino.h>
#include "uartProtocol.h"

UARTProtocol uart(Serial);

int counter = 1;

void(* resetFunc) (void) = 0;

void uartCommand(byte cmd, byte id, byte *data, byte len) {
}

void setup() {
  Serial.begin(9600);
  uart.begin(uartCommand);
}

void loop() {
  uart.update();
}
