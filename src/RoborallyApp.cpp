#include "RoborallyApp.h"

void RoborallyApp::execute(void) {
    Headline* headline = AbstractApp::sc->getHeadline();
    TonePlayer* tonePlayer = AbstractApp::sc->getTonePlayer();

    monitorLastUpdated = millis() - noiseRefreshTimeMilis;
    roundLastUpdated = millis() - roundRefreshTimeMilis;
    gameState = CONNECTING;
    screenState = SCREEN_REFRESH_REQUIRED;
    monitorState = SCREEN_REFRESH_REQUIRED;
    messageState = SCREEN_REFRESH_REQUIRED;
    round = 0;

    while(true) {
        if (drawMainScreen()) {
            // Main screen just loaded.
            tonePlayer->playTones(AudioAssets::roborallyIntro, AudioAssets::roborallyIntroLength, false, true);
        }
        headline->update();
        
        drawRound();
        updateMonitor();
        printMessage();
        printCardNumber();
        handleKeypad();
        flashlightBlink();
        tonePlayer->play();
        if (gameState == EXIT) {
            flashlightTurnOff();
            tonePlayer->stop();
            return;
        }
    }
}

void RoborallyApp::handleKeypad(void) {
    Keypad* keypad = AbstractApp::sc->getKeypad();

    if (!keypad->read()) {
        return;
    }

    if (keypad->getKeypadSymbol() == Keypad::keyA)
    {
        //Demo. Later we will control the game menu with Keypad::keyA instead.
        gameState = EXIT;
        return;
    }

    uint8_t keypadSymbol = keypad->getKeypadSymbol();
    switch (gameState) {
        case CONNECTING:
            //Demo
            if (keypadSymbol == Keypad::keyC) {
                gameState = ENTERING_CARD;
                round = 1; //Start the game
                TonePlayer* tonePlayer = AbstractApp::sc->getTonePlayer();
                tonePlayer->playTones(AudioAssets::roborallyAction, AudioAssets::roborallyActionLength, false, true);
                flashlightTurnOn();
            }
            break;
        case ENTERING_CARD:
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

            if (keypadSymbol == Keypad::keyStar && correctCardNumberEntered) {
                gameState = WAITING_OTHERS;
                break;
            }

            // Any other key
            break;
        case WAITING_OTHERS:
            //Demo
            if (keypadSymbol == Keypad::keyC) {
                gameState = YOUR_MOVE;
                TonePlayer* tonePlayer = AbstractApp::sc->getTonePlayer();
                tonePlayer->playTones(AudioAssets::roborallyAction, AudioAssets::roborallyActionLength, false, true);
                flashlightTurnOn();
            }
            // Waiting for the command from tha main unit
            break;
        case YOUR_MOVE:
            //Demo
            if (keypadSymbol == Keypad::keyStar) {
                gameState = NEXT_PHASE_WAITING;
                cardNumber = 0;
            }
            break;
        case NEXT_PHASE_WAITING:
            //Demo. Waiting for the command from tha main unit
            if (keypadSymbol == Keypad::keyC) {
                round += 1;
                if (round > 5) {
                    round = 1;
                }
                gameState = ENTERING_CARD;
                TonePlayer* tonePlayer = AbstractApp::sc->getTonePlayer();
                tonePlayer->playTones(AudioAssets::roborallyAction, AudioAssets::roborallyActionLength, false, true);
                flashlightTurnOn();
            }
            
            break;
    }
}

bool RoborallyApp::drawMainScreen(void) {
    if (screenState != SCREEN_REFRESH_REQUIRED) {
        return false;
    }
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    ByteLoader* byteLoader32 = AbstractApp::sc->getByteLoader32();
    Headline* headline = AbstractApp::sc->getHeadline();
    MenuRenderer* menuRenderer = AbstractApp::sc->getMenuRenderer();

    lcd->clear(false);
    headline->update(true);
    menuRenderer->render_header(StringAssets::loading);
    ProgressBar* progressBar = new ProgressBar(lcd, 10, 74, 3, true);
    progressBar->render(0);

    unsigned char* bitmap = new unsigned char[LcdAssets::roborallyMainScreenLength];

    for (unsigned int i = 0; i < LcdAssets::roborallyMainScreenLength; i++) {
        byteLoader32->loadByteToPosition(bitmap, LcdAssets::roborallyMainScreenAddress + i, i);
        progressBar->render(i * 100 / LcdAssets::roborallyMainScreenLength);
        headline->update();
    }
    
    lcd->setCursor(0, 1);
    lcd->draw(bitmap, LcdAssets::roborallyMainScreenLength, false);
    delete bitmap;
    delete progressBar;
    screenState = gameState;

    return true;
}

void RoborallyApp::drawRound() {
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

void RoborallyApp::printCardNumber() {
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    lcd->setCursor(4, 2);
    if (gameState != ENTERING_CARD) {
        for (uint8_t i = 0; i < 4; i++) {
            lcd->print("-");
        }
    } else {
        for (uint8_t i = 4; i > 0; i--) {
            if (pow(10, i - 1) <= cardNumber) {
                lcd->print(cardNumber);
                break;
            }
            lcd->print("_");
        }
    }
}

void RoborallyApp::printMessage() {
    if (gameState == messageState) {
        return;
    }

    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    lcd->setCursor(3, 5);
    for (uint8_t i = 0; i < 13; i++) {
        lcd->print(" ");
    }
    lcd->setCursor(3, 5);
    switch (gameState) {
        case CONNECTING:
            lcd->print(StringAssets::connecting);
            break;
        case ENTERING_CARD:
            lcd->print(StringAssets::enterCard);
            break;
        case WAITING_OTHERS:
        case NEXT_PHASE_WAITING:
            lcd->print(StringAssets::waiting);
            break;
        case YOUR_MOVE:
            lcd->print(StringAssets::yourMove);
            break;
        case POWER_DOWN:
            lcd->print(StringAssets::yourMove);
            break;
        default:
            break;
    }
    messageState = gameState;
}

void RoborallyApp::updateMonitor(void) {
    if (gameState == monitorState 
        && gameState != ENTERING_CARD 
        && gameState != WAITING_OTHERS 
        && gameState != NEXT_PHASE_WAITING 
        && gameState != CONNECTING
    ) {
        return;
    }

    unsigned short int monitorRefreshTimeMilis = (gameState == CONNECTING || gameState == WAITING_OTHERS || gameState == NEXT_PHASE_WAITING)
        ? waitingRefreshTimeMilis
        : noiseRefreshTimeMilis
    ;

    if (millis() - monitorLastUpdated < monitorRefreshTimeMilis) {
        return;
    }

    unsigned char* bitmap = new unsigned char[LcdAssets::roborallyMovesBitmapLength];

    ByteLoader* bitmapLoader = AbstractApp::sc->getByteLoader32();
    switch (gameState) {
        case CONNECTING:
        case WAITING_OTHERS:
        case NEXT_PHASE_WAITING:
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
        case POWER_DOWN:
            bitmapLoader->loadBitmap(
                bitmap, 
                LcdAssets::roborallyMovePowerDownAddress, 
                LcdAssets::roborallyMovesBitmapLength
            );
            break;
        case ENTERING_CARD:
        case YOUR_MOVE:
            if (cardNumber > maxCardNumber) {
                generateNoise(bitmap);
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
                case U_TURN:
                    moveAddress = LcdAssets::roborallyMoveUTurnAddress;
                    break;
                case BACKUP:
                    moveAddress = LcdAssets::roborallyMoveBackAddress;
                    break;
                case ROTATE_RIGHT:
                    moveAddress = LcdAssets::roborallyMoveRightAddress;
                    break;
                case ROTATE_LEFT:
                    moveAddress = LcdAssets::roborallyMoveLeftAddress;
                    break;
                case FORWARD:
                    moveAddress = LcdAssets::roborallyMoveForwardAddress;
                    break;
                case FORWARD_2:
                    moveAddress = LcdAssets::roborallyMoveForward2Address;
                    break;
                case FORWARD_3:
                    moveAddress = LcdAssets::roborallyMoveForward3Address;
                    break;
            }

            if (moveAddress == 0) {
                // unknown card
                generateNoise(bitmap);
                correctCardNumberEntered = false;
                break;
            }

            correctCardNumberEntered = true;
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
        ? flashlightAAddress
        : flashlightBAddress
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