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
#include "ContrastCtlApp.h"
#include "ClockSetupApp.h"
#include "RadioChannelApp.h"
#include "RadioChannelScanApp.h"
#include "RoborallyApp.h"
#include <TonePlayer.h>
#include <Nokia_LCD.h>
#include <ConfigStorage.h>

void(* resetFunc) (void) = 0;
bool writeBytes(Eeprom24C* eeprom24C, unsigned int address, unsigned int totalCapacity, unsigned short int length, const unsigned char* bytes, bool doubleRows);
void drawFullScreen(const unsigned short int length, unsigned int address);