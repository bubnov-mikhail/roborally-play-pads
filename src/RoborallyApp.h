#pragma once

#include "AbstractApp.h"

class RoborallyApp : public AbstractApp
{
public:
     RoborallyApp(unsigned short int _padNumber);
     AbstractApp::APPS execute(void);
     enum GameState
     {
          OFFLINE = 0,
          CONNECTING,
          CONNECTED,
          ENTERING_CARD,
          WAITING_QUORUM,
          WAITING_HIGHER_CARD_NUMBER,
          YOUR_MOVE,
          NEXT_PHASE_WAITING,
          POWER_DOWN,
          REFRESH_REQUIRED,
          EXIT_CONFIRMATION,
          DO_EXIT
     };
     enum Moves
     {
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
     const static uint16_t powerDownCardNumber = 1000;
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
     const static unsigned short int flashlightBlinkMillis = 100;
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
     const uint8_t addresses[maxPlayers][6] = {
         "Prime", "2Node", "3Node", "4Node", "5Node", "6Node"};
     PlayPad playPads[maxPlayers];

     /**
         * Retern pointer the the pad of the device
         */
     PlayPad *getSelf();

     /**
         * Draw the background image. Should be called once.
         */
     void drawMainScreen(unsigned short int length, unsigned int address, uint8_t lcdY);

     /**
         * Draw dots for rounds
         */
     void drawRound();

     /**
         * Draw custom text in the card area
         */
     void printCardMonitor();

     /**
         * Draw custom test in the bottom
         */
     void printMessage();

     /**
         * Handle key pressed
         */
     void handleKeypad();

     /**
         * Draw right monitor content based on the business logic
         */
     void updateMonitor(void);

     /**
         * Draw custom bitmap to the right monitor
         */
     void drawMonitor(unsigned char *bitmap);

     /**
         * Generate noise pattern
         */
     void generateNoise(unsigned char *bitmap);

     /**
         * Handle periodic flash blinking
         * @see flashlightTurnOn
         * @see flashlightTurnOff
         */
     void flashlightBlink();

     /**
         * Turn on the flash LED blinking.
         * @see flashlightBlink to handle eriodic flash blinking
         */
     void flashlightTurnOn();

     /**
         * Turn off the flash LED blinking.
         */
     void flashlightTurnOff();

     /**
         * TR/RX business logic based on the GameState of the pad
         */
     void communicate(void);

     /**
         * Setup the radio module. open reading and writing pipes
         */
     void initRadio(void);

     /**
         * Flash the light and play the sound in order the player pays attention
         */
     void activityRequired();

     /**
         * Send the device pad's GameState to all other pads
         */
     void anounceSelf(void);

     /**
         * Check if all connected pads have the same GameState
         */
     bool hasQuorum(void);

     /**
         * Set the same GameState for all not OFFLINE pads, inc. self
         */
     void setGameStatesLocaly(GameState gameState);

     /**
         * Check if the provided game state is more or equal OFFLINE and less or equal DO_EXIT
         */
     bool isValidGameState(GameState gameState);

     /**
         * Check if the provided card number is more or equal 0 and less or equal maxCardNumber
         */
     bool isValidCardNumber(uint16_t cardNumber);

     /**
         * Check if time period ends
         */
     bool isReachedTimer(unsigned long lastUpdated, unsigned long refreshTimeMilis);

     /**
         * Check how many pads have status not equal OFFLINE
         */
     uint8_t getPlayPadsConnected(void);

     /**
      * Go to the next game state
      */
     void handleGameState(void);
};