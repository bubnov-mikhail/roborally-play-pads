#pragma once

#include <Arduino.h>
#include <pins.h>
#include <BitmapLoader.h>
#include <ServiceContainer.h>
#include <buildTime.h>
#include <Keypad.h>
#include <Nokia_LCD.h>
#include <ConfigStorage.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <Headline.h>
#include "MainApp.h"

bool writeBitmap(unsigned int address, unsigned int totalCapacity, unsigned short int length, const unsigned char* bitmap);