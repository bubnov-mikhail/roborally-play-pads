#pragma once

#include "AbstractApp.h"

class RadioChannelAddressApp : public AbstractApp {
    public:
        void execute(void);
        void executeChannel(void);
        void executeAddress(void);
    private:
        void update(Nokia_LCD* lcd, uint8_t value);
        uint8_t increment(Keypad* keypad, Nokia_LCD* lcd, Headline* headline, uint8_t vMin, uint8_t vMax, uint8_t tmp, int8_t direction);
        const uint8_t channelMin = 1;
        const uint8_t channelMax = 128;
        const uint8_t addressMin = 0;
        const uint8_t addressMax = 6;
        enum Action {CHANNEL, ADDRESS};
        Action currentAction;
};