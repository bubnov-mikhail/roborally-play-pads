#pragma once

#include "AbstractApp.h"

class RadioAddressApp : public AbstractApp {
    public:
        void execute(void);
    private:
        void update(Nokia_LCD* lcd, uint8_t value);
        uint8_t increment(Keypad* keypad, Nokia_LCD* lcd, Headline* headline, uint8_t tmp, int8_t direction);
        const uint8_t vMin = 0;
        const uint8_t vMax = 6;
};