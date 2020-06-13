#pragma once

#include "AbstractApp.h"
#include "GfxAssets.h" //Remove after debug

class RoborallyApp : public AbstractApp {
    public:
        void execute(void);
    private:
        uint8_t round = 0;
        bool roundDisplayCurrent = true;
        unsigned long monitorLastUpdated;
        unsigned long roundLastUpdated;
        const static unsigned short int monitorRefreshTimeMilis = 100;
        const static unsigned short int roundRefreshTimeMilis = 400;
        void drawMainScreen(void);
        void drawRound(uint8_t _round);
        void printCardNumber(int16_t cardNumber);
        void printMessage(const char *message);
        void updateMonitor(void);
};