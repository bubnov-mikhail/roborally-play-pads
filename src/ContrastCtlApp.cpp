#include "ContrastCtlApp.h"

void ContrastCtlApp::execute(void) {
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    Keypad* keypad = AbstractApp::sc->getKeypad();
    ConfigStorage* config = AbstractApp::sc->getConfigStorage();
    MenuRenderer* menuRenderer = AbstractApp::sc->getMenuRenderer();

    uint8_t tmpContrast = config->getContrast();

    lcd->clear(false);
    menuRenderer->render_header(StringAssets::contrast);

    update(lcd, tmpContrast);

    while(true) {
        if (!keypad->read()) {
            continue;
        }
        switch (keypad->getKeypadSymbol()) {
            case Keypad::keyB:
                tmpContrast = increment(keypad, lcd, tmpContrast, 1);
                break;
            case Keypad::keyC:
                tmpContrast = increment(keypad, lcd, tmpContrast, -1);
                break;
            case Keypad::keyHash:
                tmpContrast = config->getContrast();
                update(lcd, tmpContrast);
                break;
            case Keypad::keyStar:
                config->setContrast(tmpContrast);
                return;
            case Keypad::keyD:
                tmpContrast = config->getContrast();
                update(lcd, tmpContrast);
                return;
        }
    }
}

uint8_t ContrastCtlApp::increment(Keypad* keypad, Nokia_LCD* lcd, uint8_t tmpContrast, int8_t direction)
{
    tmpContrast = min(contrastMax, max(contrastMin, tmpContrast + direction));
    update(lcd, tmpContrast);
    delay(300);
    while(true) {
        if(keypad->read()) {
            return tmpContrast;
        }
        delay(50);
        tmpContrast = min(contrastMax, max(contrastMin, tmpContrast + direction));
        update(lcd, tmpContrast);
    }

    return tmpContrast;
}

void ContrastCtlApp::update(Nokia_LCD* lcd, uint8_t value)
{
    lcd->setContrast(value);
    lcd->setCursor(10, 2);

    lcd->draw(LcdAssets::progressBarSideBorder, 1, true);
    lcd->draw(LcdAssets::progressBarEmptyBody, 1, true);
    for (int i = contrastMin; i < contrastMax; i++) {
        lcd->draw(value <= i ? LcdAssets::progressBarEmptyBody : LcdAssets::progressBarFilledBody, 1, true);
    }
    lcd->draw(LcdAssets::progressBarEmptyBody, 1, true);
    lcd->draw(LcdAssets::progressBarSideBorder, 1, true);
}