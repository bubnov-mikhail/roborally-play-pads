#pragma once

#include "AbstractApp.h"

class RadioChannelApp : public AbstractApp
{
public:
    AbstractApp::APPS execute(void);

private:
    void update(Nokia_LCD *lcd, uint8_t value);
    uint8_t increment(Keypad *keypad, Nokia_LCD *lcd, Headline *headline, uint8_t vMin, uint8_t vMax, uint8_t tmp, int8_t direction);
    const uint8_t vMin = 1;
    const uint8_t vMax = 125;
};