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

    lcd->setCursor(0, 1);
    lcd->draw(roborallyMainScreen, roborallyMainScreenWidth, true);
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

    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    lcd->setCursor(61, 2);
    for (uint8_t i = 0; i < 28; i++) {
        if (i == 14) {
            lcd->setCursor(61, 3);
        }
        lcd->draw(LcdAssets::noise[random(0, 5)], 1, true);
    }

    monitorLastUpdated = millis();
}