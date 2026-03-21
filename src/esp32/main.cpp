
#include <Arduino.h>
#include "FirebaseHandler.h"
#include "eepromManager.h"
#include "secretData.h"
#include "protocolMap.h"
#include "taskManager.h"
#include "kalmanFilter.h"
#include <esp_task_wdt.h>
#include <eepromPointer.h>
#include "servoController.h"
#include "commandParser.h"
#include "uartProtocol.h"

#define WIFI_SSID "ADAN"
#define WIFI_PASSWORD "titanasri"
#define API_KEY "AIzaSyAIBdhO3OfKWXTPZoowuRNbxNTCQ6vW2qo"
#define USER_EMAIL "esp32@betapictoris.com"
#define USER_PASSWORD "betapictorisshooterrobotesp32"
#define DATABASE_URL "https://sollarion-f7d4f-default-rtdb.asia-southeast1.firebasedatabase.app"

#define RXD2 16
#define TXD2 17


using cbyte = const byte;
using uint = unsigned int;

FirebaseHandler firebase;
EEPROMManager memory;
MutexData<int> sendNumber(0);
MutexData<int> number(0);
MutexData<String> interCoreCmdCommand("");  
MutexData<String> interCoreCmdTarget("");
MutexData<String> interCoreCmdValue("");

uint count = 0;
cbyte esp32 = 0;
cbyte nano = 1;
int test = 0;

// Command parsing
String target, command, value;

String serialInput;
UARTProtocol uart(Serial2);

const byte CMD_GET_NUMBER = 0x10;
const byte CMD_NUMBER     = 0x11;
const byte CMD_RESTART    = 0x12;

void handleCommand(byte cmd, byte id, byte *data, byte len) {
  if (cmd == CMD_NUMBER) {
    int num = (data[0] << 8) | data[1];
    number.set(num);


    // Serial.print("ID ");
    // Serial.print(id);
    // Serial.print(" = ");
    // Serial.println(num);

  }
}

bool commandRun(const String &target, const String &command, const String &value) {
    if (target == "rbt") {
        if (command == "restart") {
            uart.send(CMD_RESTART, 0, NULL);
            delay(100);
            ESP.restart();
            return true;
        }
    }

    else if (target == "esp32") {
        if (command == "restart") {
            
        }

    } else if (target == "nano") {
        if (command == "led") {
            
        }
    }

    return false;
}

// CORE 1: Main function
void mainFunction(void *pvParameters) {
    while (1) {
        uart.update();
        
        static unsigned long t = 0;
        if (millis() - t > 1000) {
            uart.send(CMD_GET_NUMBER, 0, NULL);
            t = millis();
        }

        serialInput = Serial.readStringUntil('\n');
        serialInput.trim();
        parseCmd(serialInput, target, command, value);

        if (target == "") {
            Serial.println("Invalid command target");
        } else if (command == "") {
            Serial.println("Invalid command");
        } else {
            interCoreCmdCommand.set(command);
            interCoreCmdTarget.set(target);
            interCoreCmdValue.set(value);
            commandRun(target, command, value);
        }

        
        
        


        static unsigned long lastTime = 0;
        static unsigned long loopCount = 0;

        loopCount++;

        if (millis() - lastTime >= 1000) {
            Serial.print("Loop rate core1: ");
            Serial.print(loopCount);
            Serial.println(" Hz");

            loopCount = 0;
            lastTime = millis();
        }
        


        vTaskDelay(5 / portTICK_PERIOD_MS);
        yield();
    }
}

void internet(void *pvParameters) {
    static long lastMillis = 0;
    while (1) {


        if (millis() - lastMillis >= 400) {
            lastMillis = millis();
            firebase.sendInt("/test/sensor", number.get());
            esp_task_wdt_reset();
            parseCmd(firebase.getString("/test/command"), target, command, value);
            esp_task_wdt_reset();
            if (command != ""){ Serial.println(command); }
            count ++;
        }

        


        vTaskDelay(15 / portTICK_PERIOD_MS);
        yield();
    }
}

void runtime(void *pvParameters) {
    while (1) {
        
        vTaskDelay(5 / portTICK_PERIOD_MS);
        yield();
    }
}

void output(cbyte device, const String &msg) {
    static String deviceName = "ESP32";
    byte core = 0;
    if (msg == "") return;
    if (device == 0) {
        deviceName = "ESP32";
        core = xPortGetCoreID();
    } else if (device == 1) {
        deviceName = "NANO";
    } else {
        deviceName = "UNKNOWN";
    }
    
    Serial.print(">> ");
    Serial.print(deviceName);
    if (device == 0) {
        Serial.print(core);
    }
    Serial.print(": ");
    Serial.println(msg);
}

void setup(){

    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
    uart.begin(handleCommand);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while(WiFi.status() != WL_CONNECTED){
        delay(300);
    }

    firebase.begin(
        API_KEY,
        DATABASE_URL,
        USER_EMAIL,
        USER_PASSWORD
    );

    memory.begin(512);
    memory.save('q',"DATA");
    memory.save('a',"HELLO");
    String data = memory.get('q');
    memory.remove('q');
    Serial.println(data);
    Serial.println("ALL DATA:");
    Serial.println(memory.getAll());

    esp_task_wdt_init(5, true);

    // CORE 0
    xTaskCreatePinnedToCore(
        internet,
        "internet task",
        12000,
        NULL,
        1,
        NULL,
        0
    );

    // CORE 1
    xTaskCreatePinnedToCore(
        mainFunction,
        "main task",
        2048,
        NULL,
        1,
        NULL,
        1
    );

    xTaskCreatePinnedToCore(
        runtime,
        "runtime task",
        2048,
        NULL,
        1,
        NULL,
        1
    );

}

void loop(){}


//////////////////////////////////////////////////////////////





