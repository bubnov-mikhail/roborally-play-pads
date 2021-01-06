#pragma once

#include <Arduino.h>

#define PIN_KEYPAD_MOSI_CS 8
#define PIN_KEYPAD_MISO_DATA 7 //Data pin
#define PIN_KEYPAD_MOSI_SCK A1 // Clk
#define PIN_BUZZER 6

// 5110
#define PIN_NOKIA_DC 4 // Data/Command select (D/C)
#define PIN_NOKIA_BL 2 // Backlight
#define PIN_NOKIA_CE 3 // Chip Select
#define PIN_NOKIA_RST 5 // Reset

// Voltage reader
#define PIN_VOLTAGE_READ A0

// nRF24L01+
#define PIN_RADIO_CE 10 // Chip enabled
#define PIN_RADIO_CS 9 // Chip select
