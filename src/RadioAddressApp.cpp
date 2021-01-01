#include "RadioAddressApp.h"

void RadioAddressApp::execute(void) {
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    Keypad* keypad = AbstractApp::sc->getKeypad();
    ConfigStorage* config = AbstractApp::sc->getConfigStorage();
    MenuRenderer* menuRenderer = AbstractApp::sc->getMenuRenderer();
    Headline* headline = AbstractApp::sc->getHeadline();
    uint8_t tmp = config->getRadioAddress();

    lcd->clear(false);
    menuRenderer->render_header(StringAssets::radioAddress);
    headline->update(true);
    lcd->setCursor(0, 3);
    lcd->print(StringAssets::radioAddress);
    lcd->print(StringAssets::colon);
    update(lcd, tmp);

    while(true) {
        headline->update();
        if (!keypad->read()) {
            continue;
        }
        switch (keypad->getKeypadSymbol()) {
            case Keypad::keyB:
                tmp = increment(keypad, lcd, headline, tmp, 1);
                break;
            case Keypad::keyC:
                tmp = increment(keypad, lcd, headline, tmp, -1);
                break;
            case Keypad::keyHash:
                // Reset to config
                tmp = config->getRadioChannel();
                update(lcd, tmp);
                break;
            case Keypad::keyStar:
                // Save and exit
                config->setRadioAddress(tmp);
                return;
            case Keypad::keyD:
                // Exit without saving
                return;
        }
    }
}


uint8_t RadioAddressApp::increment(Keypad* keypad, Nokia_LCD* lcd, Headline* headline, uint8_t tmp, int8_t direction)
{
    tmp = min(vMax, max(vMin, tmp + direction));
    update(lcd, tmp);
    unsigned long longDelay = millis();
    unsigned long shortDelay;

    while(true) {
        headline->update();
        if(keypad->read()) {
            return tmp;
        }

        if ((millis() - longDelay) < 300) {
            continue;
        } else if(longDelay > 0) {
            longDelay = 0;
            shortDelay = millis();
        }

        if ((millis() - shortDelay) < 50) {
            continue;
        }
        shortDelay = millis();
        tmp = min(vMax, max(vMin, tmp + direction));
        update(lcd, tmp);
    }

    return tmp;
}

void RadioAddressApp::update(Nokia_LCD* lcd, uint8_t value)
{
    lcd->setCursor(54, 3);
    lcd->print(StringAssets::space);
    lcd->print(StringAssets::space);
    lcd->print(StringAssets::space);
    lcd->setCursor(48, 3);

    lcd->print(value);
}