#pragma once

#include "AbstractApp.h"

class RoborallyApp : public AbstractApp {
    public:
        RoborallyApp(unsigned short int _padNumber);
        AbstractApp::APPS execute(void);
        enum GameState {
            OFFLINE, 
            CONNECTING, 
            CONNECTED, 
            ENTERING_CARD, 
            WAITING_OTHERS, 
            YOUR_MOVE, 
            NEXT_PHASE_WAITING, 
            POWER_DOWN, 
            REFRESH_REQUIRED, 
            EXIT_CONFIRMATION, 
            DO_EXIT
        };
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
        static uint8_t spiMoSiCs; // Used to flash lights using spi
    private:
        uint16_t cardNumber = 0;
        const uint16_t powerDownCardNumber = 1000;
        uint8_t waitingPhase = 0;
        GameState statePriorExitConfirmation;
        GameState screenState;
        GameState monitorState;
        GameState messageState;
        uint8_t round;
        bool validCardNumberEntered = false;
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
        const static uint16_t maxCardNumber = 1000;
        const static uint8_t waitingPhaseMax = 6;

        unsigned short int playPadNumber = 0;
        const static unsigned short int maxPlayers = 6;
        struct PlayPad
        {
            GameState state = OFFLINE;
            uint16_t cardNumber = 0;
        };
        const uint64_t addresses[maxPlayers] = {
            0xABABABABAB,
            0xCDCDCDCD00,
            0xCDCDCDCD11,
            0xCDCDCDCD22,
            0xCDCDCDCD33,
            0xCDCDCDCD44,
        };
        PlayPad playPads[maxPlayers];

        PlayPad* getSelf();
        GameState getGameState();
        void drawMainScreen(unsigned short int length, unsigned int address, uint8_t lcdY);
        void drawRound();
        void printCardMonitor();
        void printMessage();
        void handleKeypad();
        void updateMonitor(void);
        void drawMonitor(unsigned char* bitmap);
        void generateNoise(unsigned char* bitmapUpper);
        void flashlightBlink();
        void flashlightTurnOn();
        void flashlightTurnOff();
        void communicate(void);
        void initRadio(void);
        void startRound(uint8_t _round);
        uint8_t getPlayPadsConnected(void);
};