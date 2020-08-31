#pragma once

#include <Nokia_LCD.h>
#include <LcdAssets.h>

class ProgressBar {
    public:
        ProgressBar(Nokia_LCD* _lcd, uint8_t _min, uint8_t _max, uint8_t _lcdY, bool _riseOnly);
        void render(uint8_t percent);
        void reset();
    private:
        Nokia_LCD* lcd;
        uint8_t min;
        uint8_t max;
        uint8_t lastValue;
        uint8_t lcdY;
        bool riseOnly;
        bool initRequired = true;
};