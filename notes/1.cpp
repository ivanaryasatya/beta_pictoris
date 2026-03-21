

#include <Arduino.h>
#define RXD2 16
#define TXD2 17

const byte HEADER = 0xAA;
const byte FOOTER = 0x55;

const byte CMD_GET_NUMBER = 0x10;
const byte CMD_NUMBER     = 0x11;

byte packetID = 0;

// retry system
bool waitingResponse = false;
byte lastID = 0;
unsigned long sendTime = 0;
byte retryCount = 0;

const unsigned long TIMEOUT = 200; // ms
const byte MAX_RETRY = 3;

// =====================
// CHECKSUM
// =====================
byte calcChecksum(byte cmd, byte id, byte len, byte *data) {
  byte sum = cmd ^ id ^ len;
  for (byte i = 0; i < len; i++) sum ^= data[i];
  return sum;
}

// =====================
// SEND
// =====================
void sendCommand(byte cmd, byte len, byte *data) {

  byte checksum = calcChecksum(cmd, packetID, len, data);

  Serial2.write(HEADER);
  Serial2.write(cmd);
  Serial2.write(packetID);
  Serial2.write(len);

  for (byte i = 0; i < len; i++) {
    Serial2.write(data[i]);
  }

  Serial2.write(checksum);
  Serial2.write(FOOTER);

  lastID = packetID;
  packetID++;
  waitingResponse = true;
  sendTime = millis();
}

// =====================
// PARSER
// =====================
byte buffer[10];
byte index = 0, len = 0, cmd, id, checksum;

enum State {
  WAIT_HEADER, READ_CMD, READ_ID, READ_LEN,
  READ_DATA, READ_CHECKSUM, WAIT_FOOTER
};

State state = WAIT_HEADER;

void parseUART(byte b) {
  switch (state) {

    case WAIT_HEADER:
      if (b == HEADER) state = READ_CMD;
      break;

    case READ_CMD:
      cmd = b;
      state = READ_ID;
      break;

    case READ_ID:
      id = b;
      state = READ_LEN;
      break;

    case READ_LEN:
      len = b;
      index = 0;
      state = (len > 0) ? READ_DATA : READ_CHECKSUM;
      break;

    case READ_DATA:
      buffer[index++] = b;
      if (index >= len) state = READ_CHECKSUM;
      break;

    case READ_CHECKSUM:
      checksum = b;
      state = WAIT_FOOTER;
      break;

    case WAIT_FOOTER:
      if (b == FOOTER) {
        byte calc = calcChecksum(cmd, id, len, buffer);
        if (calc == checksum) {
          handlePacket(cmd, id, buffer, len);
        }
      }
      state = WAIT_HEADER;
      break;
  }
}

// =====================
// HANDLE
// =====================
void handlePacket(byte cmd, byte id, byte *data, byte len) {
  if (cmd == CMD_NUMBER && id == lastID) {
    int num = (data[0] << 8) | data[1];

    Serial.print("OK ID ");
    Serial.print(id);
    Serial.print(" = ");
    Serial.println(num);

    waitingResponse = false;
    retryCount = 0;
  }
}

// =====================
// SETUP
// =====================
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

// =====================
// LOOP
// =====================
void loop() {

  // kirim jika tidak sedang menunggu
  if (!waitingResponse) {
    sendCommand(CMD_GET_NUMBER, 0, NULL);
  }

  // timeout retry
  if (waitingResponse && millis() - sendTime > TIMEOUT) {

    if (retryCount < MAX_RETRY) {
      Serial.println("Retry...");
      retryCount++;
      packetID = lastID; // kirim ulang ID sama
      sendCommand(CMD_GET_NUMBER, 0, NULL);
    } else {
      Serial.println("Gagal!");
      waitingResponse = false;
      retryCount = 0;
    }
  }

  // baca UART
  while (Serial2.available()) {
    parseUART(Serial2.read());
  }
}