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

class ConfigStorage {
    public:
        ConfigStorage(void);
        ConfigStorage(bool _withBacklight, bool _withSounds, uint8_t _contrast, uint8_t _radioChannel, uint8_t _radioLevel);
        bool isWithBacklight();
        bool isWithSounds();
        uint8_t getContrast();
        uint8_t getRadioChannel();
        uint8_t getRadioLevel();
        void setWithBacklight(bool value);
        void setWithSounds(bool value);
        void setContrast(uint8_t value);
        void setRadioChannel(uint8_t value);
        void setRadioLevel(uint8_t value);
        void setRadioConnected(bool value);
        bool isRadioConnected();
    protected:
        int withBacklightAddress;
        int withSoundsAddress;
        int contrastAddress;
        int radioChannelAddress;
        int radioLevelAddress;
        bool withBacklight;
        bool withSounds;
        uint8_t contrast;
        uint8_t radioChannel;
        uint8_t radioLevel;
        bool radioConnected;
        void initAddresses(void);
};