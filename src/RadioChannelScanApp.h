#pragma once

#include "AbstractApp.h"

class RadioChannelScanApp : public AbstractApp {
    public:
        AbstractApp::APPS execute(void);
    private:
        const uint8_t channelMin = 1;
        const uint8_t channelMax = 128;
        uint8_t getBestChannel();
};