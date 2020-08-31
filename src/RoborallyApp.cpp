#include "RoborallyApp.h"

void RoborallyApp::execute(void) {
    Keypad* keypad = AbstractApp::sc->getKeypad();
    Headline* headline = AbstractApp::sc->getHeadline();

    monitorLastUpdated = millis() - monitorRefreshTimeMilis;
    roundLastUpdated = millis() - roundRefreshTimeMilis;
    gameState = CONNECTING;
    screenState = SCREEN_REFRESH_REQUIRED;
    monitorState = SCREEN_REFRESH_REQUIRED;
    messageState = SCREEN_REFRESH_REQUIRED;
    round = 0;

    while(true) {
        drawMainScreen();
        headline->update();
        
        drawRound();
        updateMonitor();
        printMessage();
        printCardNumber();

        if (!keypad->read()) {
            continue;
        }

        switch (keypad->getKeypadSymbol()) {
            case Keypad::keyC: //Demo
                gameState = ENTERING_CARD;
                break;
            case Keypad::keyD: //Demo
                return;
        }
        
        screenState = gameState;
    }
}

void RoborallyApp::drawMainScreen(void) {
    if (screenState != SCREEN_REFRESH_REQUIRED) {
        return;
    }
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    BitmapLoader* bitmapLoader = AbstractApp::sc->getBitmapLoader();
    Headline* headline = AbstractApp::sc->getHeadline();
    MenuRenderer* menuRenderer = AbstractApp::sc->getMenuRenderer();

    lcd->clear(false);
    headline->update(true);
    menuRenderer->render_header(StringAssets::loading);

    unsigned char* bitmap = new unsigned char[LcdAssets::roborallyMainScreenLength];
    bitmapLoader->loadBitmap(bitmap, LcdAssets::roborallyMainScreenAddress, LcdAssets::roborallyMainScreenLength);
    lcd->setCursor(0, 1);
    lcd->draw(bitmap, LcdAssets::roborallyMainScreenLength, false);
    delete bitmap;
    screenState = gameState;
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
    if (renderedCardNumber == cardNumber) {
        return;
    }
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    lcd->setCursor(4, 2);
    if (gameState != ENTERING_CARD && gameState != CONFIRMING_CARD) {
        for (uint8_t i = 0; i < 4; i++) {
            lcd->print("-");
        }
    } else {
        for (uint8_t i = 0; i < 4; i++) {
            lcd->print("_");
        }
        if (cardNumber > 0) {
            lcd->setCursor(4, 2);
            lcd->print(cardNumber);
        }
    }
    
    renderedCardNumber = cardNumber;
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
        case CONFIRMING_CARD:
            lcd->print(StringAssets::confirmCard);
            break;
        case WAITING:
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
    if (gameState == monitorState && gameState != ENTERING_CARD && gameState != WAITING && gameState != CONNECTING) {
        return;
    }

    if (millis() - monitorLastUpdated < monitorRefreshTimeMilis) {
        return;
    }
    
    unsigned char* bitmapUpper = new unsigned char[monitorBitmapLength];
    unsigned char* bitmapLower = new unsigned char[monitorBitmapLength];  

    //switch state
    // other cards

    // fallback to noise
    generateNoise(bitmapUpper);
    generateNoise(bitmapLower);
    
    drawMonitor(bitmapUpper, bitmapLower);
    delete bitmapUpper;
    delete bitmapLower;
    monitorLastUpdated = millis();
    monitorState = gameState;
}

void RoborallyApp::drawMonitor(unsigned char* bitmapUpper, unsigned char* bitmapLower) {
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    lcd->setCursor(61, 2);
    lcd->draw(bitmapUpper, monitorBitmapLength, false);
    lcd->setCursor(61, 3);
    lcd->draw(bitmapLower, monitorBitmapLength, false);
}

void RoborallyApp::generateNoise(unsigned char* bitmap) {
    for (uint8_t i = 0; i < monitorBitmapLength; i++) {
        bitmap[i] = LcdAssets::noise[random(0, 5)];
    }
}