#pragma once

#include <Arduino.h>
#include <pins.h>
#include <ByteLoader.h>
#include <ServiceContainer.h>
#include <buildTime.h>
#include <Keypad.h>
#include <Nokia_LCD.h>
#include <ConfigStorage.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Headline.h>
#include "MainApp.h"

//#define DEBUG true
//#define SET_CONFIG_DEFAULTS true
#define SET_GFX_ASSETS true
#define Eeprom24C32_capacity 32 // Size in bytes
#define Eeprom24C32_address 0x50
#define Eeprom24C08_capacity 8 // Size in bytes
#define Eeprom24C08_address 0x54
#if defined(SET_GFX_ASSETS)
  #include "GfxAssets.h"
  #include <ProgressBar.h>
#endif

bool writeBytes(Eeprom24C* eeprom24C, unsigned int address, unsigned int totalCapacity, unsigned short int length, const unsigned char* bytes);