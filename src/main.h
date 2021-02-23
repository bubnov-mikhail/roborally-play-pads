#pragma once

#include <Arduino.h>
#include <pins.h>
#include <ByteLoader.h>
#include <ServiceContainer.h>
#include <buildTime.h>
#include <Keypad.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include "RF24.h"
#include <Headline.h>
#include "MainApp.h"
#include <TonePlayer.h>
#include <Nokia_LCD.h>
#include <ConfigStorage.h>

bool writeBytes(Eeprom24C* eeprom24C, unsigned int address, unsigned int totalCapacity, unsigned short int length, const unsigned char* bytes, bool doubleRows);