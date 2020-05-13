#pragma once

#include <Arduino.h>
#include <pins.h>
#include <ServiceContainer.h>
#include <Keypad.h>
#include <Nokia_LCD.h>
#include <LcdAssets.h>
#include <ConfigStorage.h>
#include "MainApp.h"

void handleKeypadSymbol(uint8_t keypadSymbol);
void setContrast(void);
