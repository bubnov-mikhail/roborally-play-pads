#pragma once

#include "AbstractApp.h"

class RadioChannelApp : public AbstractApp {
    public:
        void execute(void);
    private:
        void update(Nokia_LCD* lcd, uint8_t value);
        uint8_t increment(Keypad* keypad, Nokia_LCD* lcd, Headline* headline, uint8_t tmpChannel, int8_t direction);
        const uint8_t channelMin = 1;
        const uint8_t channelMax = 128;
};