#include "RadioChannelApp.h"

void RadioChannelApp::execute(void) {
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    Keypad* keypad = AbstractApp::sc->getKeypad();
    ConfigStorage* config = AbstractApp::sc->getConfigStorage();
    MenuRenderer* menuRenderer = AbstractApp::sc->getMenuRenderer();
    Headline* headline = AbstractApp::sc->getHeadline();
    uint8_t tmpChannel = config->getRadioChannel();

    lcd->clear(false);
    menuRenderer->render_header(StringAssets::radioChannel);
    headline->update(true);
    lcd->setCursor(0, 3);
    lcd->print(StringAssets::radioChannel);
    lcd->print(StringAssets::colon);
    lcd->print(StringAssets::space);
    update(lcd, tmpChannel);

    while(true) {
        headline->update();
        if (!keypad->read()) {
            continue;
        }
        switch (keypad->getKeypadSymbol()) {
            case Keypad::keyB:
                tmpChannel = increment(keypad, lcd, headline, tmpChannel, 1);
                break;
            case Keypad::keyC:
                tmpChannel = increment(keypad, lcd, headline, tmpChannel, -1);
                break;
            case Keypad::keyHash:
                // Reset to config
                tmpChannel = config->getRadioChannel();
                update(lcd, tmpChannel);
                break;
            case Keypad::keyStar:
                // Save and exit
                config->setRadioChannel(tmpChannel);
                return;
            case Keypad::keyD:
                // Exit without saving
                return;
        }
    }
}


uint8_t RadioChannelApp::increment(Keypad* keypad, Nokia_LCD* lcd, Headline* headline, uint8_t tmpChannel, int8_t direction)
{
    tmpChannel = min(channelMax, max(channelMin, tmpChannel + direction));
    update(lcd, tmpChannel);
    unsigned long longDelay = millis();
    unsigned long shortDelay;

    while(true) {
        headline->update();
        if(keypad->read()) {
            return tmpChannel;
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
        tmpChannel = min(channelMax, max(channelMin, tmpChannel + direction));
        update(lcd, tmpChannel);
    }

    return tmpChannel;
}

void RadioChannelApp::update(Nokia_LCD* lcd, uint8_t value)
{
    lcd->setCursor(40, 3);
    lcd->setInverted(true);
    lcd->print(value);
    lcd->setInverted(false);
}