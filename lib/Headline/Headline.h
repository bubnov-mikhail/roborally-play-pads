// --------------------
// ConfigStorage - v1.0
//
// AUTHOR/LICENSE:
// Created by Mihail Bubnov- bubnov.mihail@gmail.com
// Copyright 2020 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
//
// LINKS:
// Project home: https://github.com/bubnov-mikhail/roborally-play-pads
//
// This library is using EEPROM to load and save device configs
#pragma once
#include <inttypes.h>
#include <ConfigStorage.h>
#include <Nokia_LCD.h>
#include <LcdAssets.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

class Headline {
    public:
        Headline(ConfigStorage* _configStorage, Nokia_LCD* _lcd, DS1307RTC* _rtc, uint8_t _pinVoltageRead);
        void update(bool forceUpdate = false);
    protected:
        ConfigStorage* configStorage;
        Nokia_LCD* lcd;
        DS1307RTC* rtc;
        uint8_t pinVoltageRead;
        unsigned long lastUpdated;
        bool displayClockDots = true;
        inline void updateRtc(void);
        inline void updateBattery(void);
        void printValue(uint8_t value);
        void printDigit(uint8_t value);
        const static unsigned short int refreshTimeMilis = 500;
};