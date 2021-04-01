#pragma once

#include "AbstractApp.h"
#include <ProgressBar.h>

class ContrastCtlApp : public AbstractApp {
    public:
        AbstractApp::APPS execute(void);
    private:
        void update(Nokia_LCD* lcd, ProgressBar* progressBar, uint8_t value);
        uint8_t increment(Keypad* keypad, Nokia_LCD* lcd, ProgressBar* progressBar, Headline* headline, uint8_t tmpContrast, int8_t direction);
        const uint8_t contrastMin = 20;
        const uint8_t contrastMax = 80;
};