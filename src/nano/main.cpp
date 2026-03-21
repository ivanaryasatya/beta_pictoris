#include <Arduino.h>
#include "uartProtocol.h"

UARTProtocol uart(Serial);

const byte CMD_GET_NUMBER = 0x10;
const byte CMD_NUMBER     = 0x11;
const byte CMD_RESTART    = 0x12;

int counter = 1;

void(* resetFunc) (void) = 0;

void handleCommand(byte cmd, byte id, byte *data, byte len) {

  if (cmd == CMD_GET_NUMBER) {

    byte d[2];
    d[0] = (counter >> 8);
    d[1] = counter & 0xFF;

    uart.send(CMD_NUMBER, 2, d);
    counter++;
  } else if (cmd == CMD_RESTART) {
    Serial.println("Restart nano...");
    resetFunc();

    while (1); // tunggu reset
  }
}

void setup() {
  Serial.begin(9600);
  uart.begin(handleCommand);
}

void loop() {
  uart.update();
}