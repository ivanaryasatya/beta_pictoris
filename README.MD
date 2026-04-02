# 1. ESP32-DevKitC V4 (ESP32-WROOM-32U Module)

This board is the official high-performance development board from Espressif. The letter "U" in 32U stands for U.FL, which means this module does not have a printed antenna on the PCB. Instead, it uses an external connector (IPEX/U.FL) to attach a Wi-Fi/Bluetooth antenna. You **must** attach an antenna for optimal signal functionality.

## Main Specifications
* **Microcontroller:** ESP32-D0WD Chip (Tensilica Xtensa Dual-Core 32-bit Architecture).
* **Clock Speed:** Adjustable up to 240 MHz.
* **Operating Voltage (Logic):** 3.3V.
* **Input Voltage (Power):** 5V via USB port, or 5V - 12V via the VIN pin (Maximum 9V recommended to prevent the voltage regulator from overheating).
* **Wireless Connectivity:** Wi-Fi (802.11 b/g/n) and Bluetooth (v4.2 BR/EDR and BLE).
* **Flash Memory:** 4 MB (Standard, integrated inside the WROOM module).
* **SRAM:** 520 KB.
* **USB-to-Serial Chip:** Typically uses CP2102 or CH340 (depending on the board manufacturer).
* **Antenna Connector:** U.FL / IPEX (External antenna must be purchased separately).

## Pin Specifications (I/O)
* **Digital I/O Pins:** 34 pins (most support PWM).
* **Analog Input Pins (ADC):** 18 channels (12-bit resolution).
* **Analog Output Pins (DAC):** 2 channels (8-bit resolution).
* **Interface Support:** 3x UART, 3x SPI, 2x I2C, 2x I2S, CAN bus, and capacitive touch sensors (Touch Pins).

---

# 2. Arduino Nano ATmega168P (Type-C Version)

This is a standard AVR microcontroller version which is perfectly suited for simple projects that do not require an internet connection or heavy computing. This Type-C clone board updates its physical connection port, but its main brain (ATmega168P) has significantly smaller memory compared to its bigger brother, the ATmega328P.

## Main Specifications
* **Microcontroller:** Atmel ATmega168P (AVR 8-bit Architecture).
* **Clock Speed:** 16 MHz.
* **Operating Voltage (Logic):** 5V.
* **Input Voltage (Power):** 5V via USB Type-C, or 7V - 12V via the VIN pin.
* **Flash Memory:** 16 KB (Approximately 2 KB is reserved for the bootloader, leaving 14 KB for your code).
* **SRAM:** 1 KB.
* **EEPROM:** 512 Bytes (Used to store data permanently without electricity).
* **USB-to-Serial Chip:** Almost exclusively uses CH340 (You may need to install the CH340 driver on your computer for it to be detected).

## Pin Specifications (I/O)
* **Digital I/O Pins:** 14 pins.
* **PWM Support:** 6 pins (Pins 3, 5, 6, 9, 10, 11).
* **Analog Input Pins:** 8 pins (A0 to A7) with 10-bit resolution.
* **Interface Support:** 1x UART (Pins 0 and 1), 1x I2C (Pins A4 and A5), 1x SPI.

---

# Quick Comparison

The following table highlights the most prominent differences if you are considering which board to use for a specific project:

| Feature / Component | ESP32-DevKitC (32U) | Arduino Nano (168P) |
| :--- | :--- | :--- |
| **CPU Architecture** | 32-bit (Dual-Core) | 8-bit (Single-Core) |
| **Logic / Sensor Voltage** | 3.3V | 5V |
| **Code Capacity (Flash)** | 4000 KB (4 MB) | 14 KB (Extremely small) |
| **Internet Connection** | Yes (Wi-Fi & Bluetooth) | No |
| **Antenna Requirement** | Required (External U.FL) | Not required |