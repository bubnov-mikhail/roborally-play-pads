#include "RoborallyApp.h"

void RoborallyApp::execute(void) {
    Keypad* keypad = AbstractApp::sc->getKeypad();
    Headline* headline = AbstractApp::sc->getHeadline();
    monitorLastUpdated = millis() - monitorRefreshTimeMilis;
    roundLastUpdated = millis() - roundRefreshTimeMilis;

    drawMainScreen();
    printCardNumber(1789);
    printMessage(StringAssets::enterCard);

    while(true) {
        headline->update();
        drawRound(3);
        updateMonitor();
        keypad->read();
        if (keypad->getKeypadSymbol() == Keypad::keyD) {
            return;
        }
    }
}

void RoborallyApp::drawMainScreen(void) {
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    BitmapLoader* bitmapLoader = AbstractApp::sc->getBitmapLoader();

    unsigned char* bitmap = new unsigned char[LcdAssets::roborallyMainScreenLength];
    bitmapLoader->loadBitmap(bitmap, LcdAssets::roborallyMainScreenAddress, LcdAssets::roborallyMainScreenLength);
    lcd->setCursor(0, 1);
    lcd->draw(bitmap, LcdAssets::roborallyMainScreenLength, false);
    delete bitmap;
}

void RoborallyApp::drawRound(uint8_t _round) {
    if (millis() - roundLastUpdated < roundRefreshTimeMilis) {
        return;
    }
    roundDisplayCurrent = !roundDisplayCurrent;

    Nokia_LCD* lcd = AbstractApp::sc->getLcd();

    for (uint8_t x = 0; x < 5; x++) {
        lcd->setCursor(x * 12 + 5, 4);
        if (x == _round - 1) {
            lcd->draw(roundDisplayCurrent ? LcdAssets::roborallyRoundFilled : LcdAssets::roborallyRoundEmpty, 2, true);
            continue;
        }
        lcd->draw(x <= _round - 1 ? LcdAssets::roborallyRoundFilled : LcdAssets::roborallyRoundEmpty, 2, true);
    }

    roundLastUpdated = millis();
}

void RoborallyApp::printCardNumber(int16_t cardNumber) {
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    lcd->setCursor(4, 2);
    lcd->print(cardNumber);
}

void RoborallyApp::printMessage(const char *message) {
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    lcd->setCursor(3, 5);
    lcd->print(message);
}

void RoborallyApp::updateMonitor(void) {
    if (millis() - monitorLastUpdated < monitorRefreshTimeMilis) {
        return;
    }

    unsigned char* bitmapUpper = new unsigned char[monitorBitmapLength];
    unsigned char* bitmapLower = new unsigned char[monitorBitmapLength];

    //switch
    // other cards

    // fallback to noise
    generateNoise(bitmapUpper);
    generateNoise(bitmapLower);
    
    drawMonitor(bitmapUpper, bitmapLower);
    delete bitmapUpper;
    delete bitmapLower;
    monitorLastUpdated = millis();
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