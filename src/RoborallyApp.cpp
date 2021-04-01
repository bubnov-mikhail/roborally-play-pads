#include "RoborallyApp.h"
#include <ByteLoader.h>
#include <SPI.h>
#include <ProgressBar.h>
#include <AudioAssets.h>

RoborallyApp::RoborallyApp(unsigned short int _playPadNumber) {
    playPadNumber = _playPadNumber;
}

AbstractApp::APPS RoborallyApp::execute(void) {
    Headline* headline = AbstractApp::sc->getHeadline();
    TonePlayer* tonePlayer = AbstractApp::sc->getTonePlayer();
    Keypad* keypad = AbstractApp::sc->getKeypad();

    monitorLastUpdated = millis() - noiseRefreshTimeMilis;
    roundLastUpdated = millis() - roundRefreshTimeMilis;
    screenState = GameState::REFRESH_REQUIRED;
    monitorState = GameState::REFRESH_REQUIRED;
    messageState = GameState::REFRESH_REQUIRED;
    round = 0;

    drawMainScreen(LcdAssets::roborallyMainScreenLength, LcdAssets::roborallyMainScreenAddress, 1);
    tonePlayer->playTones(AudioAssets::roborallyIntro, AudioAssets::roborallyIntroLength, false);

    initRadio();
    PlayPad* self = getSelf();
    while(true) {
        headline->update();
        communicate();
        drawRound();
        updateMonitor();
        printMessage();
        printCardMonitor();
        handleKeypad();
        flashlightBlink();
        // tonePlayer->play(); // In case of a back music
        if (self->state == GameState::DO_EXIT) {
            flashlightTurnOff();
            // tonePlayer->stop(); // In case of a back music
            AbstractApp::sc->getLcd()->clear(false);
            AbstractApp::sc->getRadio()->powerDown();
            return AbstractApp::APPS::MAIN_MENU;
        }
    }
}

RoborallyApp::PlayPad* RoborallyApp::getSelf() {
    return &playPads[playPadNumber];
}

RoborallyApp::GameState RoborallyApp::getGameState() {
    return playPads[playPadNumber].state;
}

void RoborallyApp::handleKeypad(void) {
    Keypad* keypad = AbstractApp::sc->getKeypad();
    
    if (!keypad->read()) {
        return;
    }
    
    PlayPad* self = getSelf();
    uint8_t keypadSymbol = keypad->getKeypadSymbol();
    // We can exit the app at any state
    if (keypadSymbol == Keypad::keyA)
    {
        statePriorExitConfirmation = self->state;
        self->state = GameState::EXIT_CONFIRMATION;
        return;
    }

    
    switch (self->state) {
        case GameState::EXIT_CONFIRMATION:
            if (keypadSymbol == Keypad::keyStar) {
                self->state = GameState::DO_EXIT;
                return;
            }
            if (keypadSymbol == Keypad::keyHash || keypadSymbol == Keypad::keyD) {
                self->state = statePriorExitConfirmation;
                return;
            }
            break;
        case GameState::CONNECTING:
        case GameState::CONNECTED:
            if (getPlayPadsConnected() == 1) {
                break;
            }
            //Start the game with at least 2 players, inc self
            if (keypadSymbol == Keypad::keyStar) {
                self->state = GameState::ENTERING_CARD;
                startRound(1);
            }
            break;
        case GameState::ENTERING_CARD:
            if (keypadSymbol <= Keypad::key9) {
                uint16_t cardNumberNew = cardNumber * 10 + keypadSymbol;
                if (cardNumberNew > maxCardNumber) {
                    break;
                }
                cardNumber = cardNumberNew;
                break;

            }

            if (keypadSymbol == Keypad::keyD) {
                cardNumber = 0;
                break;
            }

            if (keypadSymbol == Keypad::keyHash) {
                cardNumber = max(0, cardNumber / 10);
                break;
            }

            if (keypadSymbol == Keypad::keyStar && validCardNumberEntered) {
                self->state = GameState::WAITING_OTHERS;
                break;
            }

            // Any other key
            break;
        case GameState::WAITING_OTHERS:
            //Demo
            if (keypadSymbol == Keypad::keyC) {
                self->state = GameState::YOUR_MOVE;
                startRound(round);
            }
            // Waiting for the command from tha main unit
            break;
        case GameState::YOUR_MOVE:
            //Demo
            if (keypadSymbol == Keypad::keyStar) {
                self->state = GameState::NEXT_PHASE_WAITING;
                cardNumber = 0;
            }
            break;
        case GameState::NEXT_PHASE_WAITING:
            //Demo. Waiting for the command from tha main unit
            if (keypadSymbol == Keypad::keyC) {
                uint8_t _round = round + 1;
                if (_round > 5) {
                    _round = 1;
                }
                self->state = GameState::ENTERING_CARD;
                startRound(_round);
            }
            
            break;
    }
}

void RoborallyApp::drawRound() {
    GameState gameState = getGameState();
    if (gameState == GameState::CONNECTING || gameState == GameState::CONNECTED) {
        // DO not draw round while the game is not started yet
        return;
    }

    if (millis() - roundLastUpdated < roundRefreshTimeMilis) {
        return;
    }
    roundDisplayCurrent = !roundDisplayCurrent;

    Nokia_LCD* lcd = AbstractApp::sc->getLcd();

    for (uint8_t x = 0; x < 5; x++) {
        lcd->setCursor(x * 12 + 5, 4);
        if (round != 0 && x == round - 1) {
            lcd->draw(roundDisplayCurrent ? LcdAssets::roborallyRoundFilled : LcdAssets::roborallyRoundEmpty, 2, true);
            continue;
        }
        lcd->draw(round > 0 && (x <= round - 1) ? LcdAssets::roborallyRoundFilled : LcdAssets::roborallyRoundEmpty, 2, true);
    }

    roundLastUpdated = millis();
}

void RoborallyApp::printCardMonitor() {
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    lcd->setCursor(4, 2);
    GameState gameState = getGameState();
    switch (gameState) {
        case GameState::CONNECTING:
        case GameState::CONNECTED:
            lcd->print(StringAssets::players);
            lcd->print(StringAssets::colon);
            lcd->print(getPlayPadsConnected());
            break;
        case GameState::ENTERING_CARD:
            for (uint8_t i = 4; i > 0; i--) {
                if (pow(10, i - 1) <= cardNumber) {
                    lcd->print(cardNumber);
                    break;
                }
                lcd->print(StringAssets::space);
            }
            break;
        default:
            for (uint8_t i = 0; i < 4; i++) {
                lcd->print(StringAssets::space);
            }
    }
}

void RoborallyApp::printMessage() {
    GameState gameState = getGameState();
    if (gameState == messageState) {
        return;
    }
    
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    lcd->setCursor(3, 5);
    for (uint8_t i = 0; i < 13; i++) {
        lcd->print(StringAssets::space);
    }

    lcd->setCursor(3, 5);
    messageState = gameState;
    
    switch (gameState) {
        case GameState::CONNECTING:
            lcd->print(StringAssets::connecting);
            return;
        case GameState::CONNECTED:
            lcd->print(StringAssets::connected);
            return;
        case GameState::ENTERING_CARD:
            lcd->print(StringAssets::enterCard);
            return;
        case GameState::WAITING_OTHERS:
        case GameState::NEXT_PHASE_WAITING:
            lcd->print(StringAssets::waiting);
            return;
        case GameState::YOUR_MOVE:
            lcd->print(StringAssets::yourMove);
            return;
        case GameState::POWER_DOWN:
            lcd->print(StringAssets::yourMove);
            return;
        case GameState::EXIT_CONFIRMATION:
            lcd->print(StringAssets::exitConfirmation);
            return;
    }
}

void RoborallyApp::updateMonitor(void) {
    GameState gameState = getGameState();
    if (gameState == monitorState 
        && gameState != GameState::ENTERING_CARD 
        && gameState != GameState::WAITING_OTHERS 
        && gameState != GameState::NEXT_PHASE_WAITING 
        && gameState != GameState::CONNECTING
        && gameState != GameState::CONNECTED
        && gameState != GameState::EXIT_CONFIRMATION
    ) {
        return;
    }

    unsigned short int monitorRefreshTimeMilis = (gameState == GameState::CONNECTED || gameState == GameState::CONNECTING || gameState == GameState::WAITING_OTHERS || gameState == GameState::NEXT_PHASE_WAITING || gameState == GameState::EXIT_CONFIRMATION)
        ? waitingRefreshTimeMilis
        : noiseRefreshTimeMilis
    ;

    if (millis() - monitorLastUpdated < monitorRefreshTimeMilis) {
        return;
    }

    unsigned char* bitmap = new unsigned char[LcdAssets::roborallyMovesBitmapLength];

    ByteLoader* bitmapLoader = AbstractApp::sc->getByteLoader32();
    switch (gameState) {
        case GameState::CONNECTING:
        case GameState::CONNECTED:
        case GameState::WAITING_OTHERS:
        case GameState::NEXT_PHASE_WAITING:
        case GameState::EXIT_CONFIRMATION:
            bitmapLoader->loadBitmap(
                bitmap, 
                LcdAssets::roborallyMoveWait1Address + waitingPhase * LcdAssets::roborallyMovesBitmapLength, 
                LcdAssets::roborallyMovesBitmapLength
            );
            waitingPhase++;
            if (waitingPhase > waitingPhaseMax) {
                waitingPhase = 0;
            }
            break;
        case GameState::POWER_DOWN:
            bitmapLoader->loadBitmap(
                bitmap, 
                LcdAssets::roborallyMovePowerDownAddress, 
                LcdAssets::roborallyMovesBitmapLength
            );
            break;
        case GameState::ENTERING_CARD:
        case GameState::YOUR_MOVE:
            if (cardNumber > maxCardNumber || (cardNumber == powerDownCardNumber && round > 1)) {
                generateNoise(bitmap);
                break;
            }

            if (cardNumber == powerDownCardNumber) {
                bitmapLoader->loadBitmap(
                    bitmap, 
                    LcdAssets::roborallyMovePowerDownAddress, 
                    LcdAssets::roborallyMovesBitmapLength
                );
                break;
            }
            
            ByteLoader* cardsLoader = AbstractApp::sc->getByteLoader08();
            unsigned int moveCardAddress = cardNumber / 10;

            uint8_t moveCard = moveCardAddress * 10 == cardNumber
                ? uint8_t(cardsLoader->loadByte(cardNumber / 10))
                : 0
            ;
            
            unsigned int moveAddress = 0;
            switch (moveCard) {
                case Moves::U_TURN:
                    moveAddress = LcdAssets::roborallyMoveUTurnAddress;
                    break;
                case Moves::BACKUP:
                    moveAddress = LcdAssets::roborallyMoveBackAddress;
                    break;
                case Moves::ROTATE_RIGHT:
                    moveAddress = LcdAssets::roborallyMoveRightAddress;
                    break;
                case Moves::ROTATE_LEFT:
                    moveAddress = LcdAssets::roborallyMoveLeftAddress;
                    break;
                case Moves::FORWARD:
                    moveAddress = LcdAssets::roborallyMoveForwardAddress;
                    break;
                case Moves::FORWARD_2:
                    moveAddress = LcdAssets::roborallyMoveForward2Address;
                    break;
                case Moves::FORWARD_3:
                    moveAddress = LcdAssets::roborallyMoveForward3Address;
                    break;
            }

            if (moveAddress == 0) {
                // unknown card
                generateNoise(bitmap);
                validCardNumberEntered = false;
                break;
            }

            validCardNumberEntered = true;
            bitmapLoader->loadBitmap(
                bitmap, 
                moveAddress, 
                LcdAssets::roborallyMovesBitmapLength
            );
            break;
        default:
            // fallback to noise
            generateNoise(bitmap);
    }
    
    drawMonitor(bitmap);
    monitorLastUpdated = millis();
    monitorState = gameState;
    delete bitmap;
}

void RoborallyApp::drawMonitor(unsigned char* bitmap) {
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    lcd->setCursor(60, 2);

    unsigned char* bitmapUpper = new unsigned char[LcdAssets::roborallyMovesBitmapLength / 2];
    unsigned char* bitmapLower = new unsigned char[LcdAssets::roborallyMovesBitmapLength / 2];
    for (uint8_t i = 0; i < LcdAssets::roborallyMovesBitmapLength; i++) {
        if (i < LcdAssets::roborallyMovesBitmapLength / 2) {
            bitmapUpper[i] = bitmap[i];
        } else {
            bitmapLower[i - LcdAssets::roborallyMovesBitmapLength / 2] = bitmap[i];
        }
    }

    lcd->draw(bitmapUpper, LcdAssets::roborallyMovesBitmapLength / 2, false);
    lcd->setCursor(60, 3);
    lcd->draw(bitmapLower, LcdAssets::roborallyMovesBitmapLength / 2, false);

    delete bitmapUpper;
    delete bitmapLower;
}

void RoborallyApp::generateNoise(unsigned char* bitmap) {
    for (uint8_t i = 0; i < LcdAssets::roborallyMovesBitmapLength; i++) {
        if (i == 0 
            || i == LcdAssets::roborallyMovesBitmapLength / 2 - 1
            || i == LcdAssets::roborallyMovesBitmapLength / 2
            || i == LcdAssets::roborallyMovesBitmapLength - 1
        ) {
            bitmap[i] = 0x0;
            continue;
        }
        bitmap[i] = LcdAssets::noise[random(0, 5)];
    }
}

void RoborallyApp::flashlightBlink() {
    if (!flashlightOn) {
        return;
    }

    if (millis() - flashlightCreated > flashlightBlinkMilis * flashlightBlinks) {
        flashlightTurnOff();
        return;
    }

    uint8_t flashState = ((millis() - flashlightCreated) / flashlightBlinkMilis) % 2 == 0
        ? flashlightBAddress
        : flashlightAAddress
    ;

    if (flashlightLastState == flashState) {
        return;
    }

    SPI.begin();
    digitalWrite(spiMoSiCs, LOW);
    SPI.transfer(flashState);
    digitalWrite(spiMoSiCs, HIGH);
    SPI.end();
}

void RoborallyApp::flashlightTurnOn() {
    flashlightOn = true;
    flashlightCreated = millis();
    flashlightBlink();
}

void RoborallyApp::flashlightTurnOff() {
    flashlightOn = false;
    SPI.begin();
    digitalWrite(spiMoSiCs, LOW);
    SPI.transfer(0x00);
    digitalWrite(spiMoSiCs, HIGH);
    SPI.end();
}

void RoborallyApp::communicate(void) {
    PlayPad* self = getSelf();
    RF24* radio = AbstractApp::sc->getRadio();

    if (playPadNumber != 0) {
        radio->openReadingPipe(0, addresses[0]);
    }

    switch (self->state) {
        case GameState::OFFLINE:
            self->state = GameState::CONNECTING;

            // Init the connection to other, already online pads
            anounceSelf();
            break;
        case GameState::CONNECTING: 
        case GameState::CONNECTED: 
            // Listen to a new pads getting online
            uint8_t pipe;
            if (radio->available(&pipe)) {
                if (pipe < maxPlayers) {
                    // Valid pipe
                    GameState lastState = playPads[pipe].state;
                    radio->read(&(playPads[pipe]), sizeof(PlayPad));
                    if (lastState == playPads[pipe].state) {
                        // Stop the infinit loop
                        return;
                    }
                    switch (getPlayPadsConnected()) {
                        case 1:
                            self->state = GameState::CONNECTING; // If no one responds
                            break;
                        case maxPlayers:
                            self->state = GameState::ENTERING_CARD; // If all responds
                            break;
                        default:
                            self->state = GameState::CONNECTED; // If at leas one responds
                    }
                    anounceSelf();
                }
            }
    }
}

void RoborallyApp::anounceSelf(void) {
    PlayPad* self = getSelf();
    RF24* radio = AbstractApp::sc->getRadio();

    radio->stopListening();
    for (uint8_t i = 0; i < maxPlayers; i++) {
        if (playPadNumber == i) {
            continue;
        }
        radio->write(self, sizeof(PlayPad));
    }
    radio->startListening();
}

void RoborallyApp::startRound(uint8_t _round) {
    round = _round;
    TonePlayer* tonePlayer = AbstractApp::sc->getTonePlayer();
    tonePlayer->playTones(AudioAssets::roborallyAction, AudioAssets::roborallyActionLength, false);
    flashlightTurnOn();
}

uint8_t RoborallyApp::getPlayPadsConnected(void) {
    uint8_t connected = 0;
    for (uint8_t i = 0; i < maxPlayers; i++) {
        if (playPads[i].state != GameState::OFFLINE) {
            connected++;
        }
    }

    return connected;
}

void RoborallyApp::initRadio(void) {
    RF24* radio = AbstractApp::sc->getRadio();
    radio->powerUp();
    radio->setPayloadSize(sizeof(PlayPad));

    /**
     * According to the datasheet, the auto-retry features's delay value should
     * be "skewed" to allow the RX node to receive 1 transmission at a time.
     * So, use varying delay between retry attempts and 15 (at most) retry attempts
    */
    radio->setRetries(((playPadNumber * 3) % 12) + 3, 15);
    for (uint8_t i = 0; i < maxPlayers; i++) {
        // set pipes
        if (i == playPadNumber) {
            radio->openWritingPipe(addresses[i]);
        } else {
            radio->openReadingPipe(i, addresses[i]);
        }
    }
}

void RoborallyApp::drawMainScreen(const unsigned short int length, unsigned int address, uint8_t lcdY)
{
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    ByteLoader* byteLoader32 = AbstractApp::sc->getByteLoader32();
    Headline* headline = AbstractApp::sc->getHeadline();

    lcd->clear(false);
    headline->update(true);
    AbstractApp::sc->getMenuRenderer()->render_header(StringAssets::loading);
    ProgressBar* progressBar = new ProgressBar(lcd, 10, 74, 3, true);
    progressBar->render(0);

    unsigned char* bitmap = new unsigned char[LcdAssets::roborallyMainScreenLength];

    for (unsigned int i = 0; i < LcdAssets::roborallyMainScreenLength; i++) {
        byteLoader32->loadByteToPosition(bitmap, LcdAssets::roborallyMainScreenAddress + i, i);
        progressBar->render(i * 100 / LcdAssets::roborallyMainScreenLength);
        headline->update();
    }
    delete progressBar;
    
    lcd->setCursor(0, 1);
    lcd->draw(bitmap, LcdAssets::roborallyMainScreenLength, false);
    delete bitmap;
}