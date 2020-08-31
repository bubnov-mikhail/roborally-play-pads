#pragma once

#include "AbstractApp.h"
#include <BitmapLoader.h>

class RoborallyApp : public AbstractApp {
    public:
        void execute(void);
        enum GameStates {NOT_DEFINED, CONNECTING, ENTERING_CARD, CONFIRMING_CARD, WAITING, YOUR_MOVE, POWER_DOWN, SCREEN_REFRESH_REQUIRED};
        static GameStates gameState;
        GameStates screenState;
        GameStates monitorState;
        GameStates messageState;
        static uint8_t round;
    private:
        int16_t cardNumber = 0;
        int16_t renderedCardNumber;
        bool roundDisplayCurrent = true;
        unsigned long monitorLastUpdated;
        unsigned long roundLastUpdated;
        const static unsigned short int monitorRefreshTimeMilis = 100;
        const static unsigned short int roundRefreshTimeMilis = 400;
        const static uint8_t monitorBitmapLength = 14;
        void drawMainScreen(void);
        void drawRound();
        void printCardNumber();
        void printMessage();
        void updateMonitor(void);
        void drawMonitor(unsigned char* bitmapUpper, unsigned char* bitmapLower);
        void generateNoise(unsigned char* bitmapUpper);
};