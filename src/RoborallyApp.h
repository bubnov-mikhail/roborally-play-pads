#pragma once

#include "AbstractApp.h"
#include <ByteLoader.h>
#include <SPI.h>
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
        static uint8_t spiMoSiCs; // Used to flash lights using spi
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
        const static unsigned short int flashlightBlinkMilis = 100;
        bool flashlightOn = false;
        unsigned long flashlightCreated = 0;
        const static uint8_t flashlightBlinks = 10;
        uint8_t flashlightLastState = 0x00;
        const static uint8_t flashlightAAddress = 0x01;
        const static uint8_t flashlightBAddress = 0x32;
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
        void flashlightBlink();
        void flashlightTurnOn();
        void flashlightTurnOff();
};