#pragma once

#include "AbstractApp.h"
#include <ByteLoader.h>
#include <ProgressBar.h>

class RoborallyApp : public AbstractApp {
    public:
        void execute(void);
        enum GameStates {NOT_DEFINED, CONNECTING, ENTERING_CARD, CONFIRMING_CARD, WAITING_OTHERS, YOUR_MOVE, NEXT_PHASE_WAITING, POWER_DOWN, SCREEN_REFRESH_REQUIRED, EXIT};
        enum Moves {
            NONE = 0, 
            U_TURN = 1, 
            BACKUP = 2, 
            ROTATE_RIGHT = 3, 
            ROTATE_LEFT = 4, 
            FORWARD = 5, 
            FORWARD_2 = 6, 
            FORWARD_3 = 7
        };
        static GameStates gameState;
        GameStates screenState;
        GameStates monitorState;
        GameStates messageState;
        uint8_t round;
    private:
        uint16_t cardNumber = 0;
        uint8_t waitingPhase = 0;
        bool correctCardNumberEntered = false;
        bool roundDisplayCurrent = true;
        unsigned long monitorLastUpdated;
        unsigned long roundLastUpdated;
        const static unsigned short int noiseRefreshTimeMilis = 100;
        const static unsigned short int waitingRefreshTimeMilis = 100;
        const static unsigned short int roundRefreshTimeMilis = 400;
        const static uint16_t maxCardNumber = 840;
        const static uint8_t waitingPhaseMax = 6;
        void drawMainScreen(void);
        void drawRound();
        void printCardNumber();
        void printMessage();
        void handleKeypad();
        void updateMonitor(void);
        void drawMonitor(unsigned char* bitmap);
        void generateNoise(unsigned char* bitmapUpper);
};