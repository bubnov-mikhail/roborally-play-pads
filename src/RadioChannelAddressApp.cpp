#include "RadioChannelAddressApp.h"

void RadioChannelAddressApp::execute(void) 
{
    Nokia_LCD* lcd = AbstractApp::sc->getLcd();
    Keypad* keypad = AbstractApp::sc->getKeypad();
    ConfigStorage* config = AbstractApp::sc->getConfigStorage();
    MenuRenderer* menuRenderer = AbstractApp::sc->getMenuRenderer();
    Headline* headline = AbstractApp::sc->getHeadline();
    uint8_t vMin;
    uint8_t vMax;
    uint8_t tmp;

    lcd->clear(false);
    
    switch (currentAction) {
        case CHANNEL:
            tmp = config->getRadioChannel();
            vMin = channelMin;
            vMax = channelMax;
            menuRenderer->render_header(StringAssets::radioChannel);
            lcd->setCursor(0, 3);
            lcd->print(StringAssets::radioChannel);
            break;
        case ADDRESS:
            tmp = config->getRadioAddress();
            vMin = addressMin;
            vMax = addressMax;
            menuRenderer->render_header(StringAssets::radioAddress);
            lcd->setCursor(0, 3);
            lcd->print(StringAssets::radioAddress);
            break;
    }
    lcd->print(StringAssets::colon);
    headline->update(true);
    update(lcd, tmp);

    while(true) {
        headline->update();
        if (!keypad->read()) {
            continue;
        }
        switch (keypad->getKeypadSymbol()) {
            case Keypad::keyB:
                tmp = increment(keypad, lcd, headline, vMin, vMax, tmp, 1);
                break;
            case Keypad::keyC:
                tmp = increment(keypad, lcd, headline, vMin, vMax, tmp, -1);
                break;
            case Keypad::keyHash:
                // Reset to config
                switch (currentAction) {
                    case CHANNEL:
                        tmp = config->getRadioChannel();
                        break;
                    case ADDRESS:
                        tmp = config->getRadioAddress();
                        break;
                }
                
                update(lcd, tmp);
                break;
            case Keypad::keyStar:
                // Save and exit
                switch (currentAction) {
                    case CHANNEL:
                        config->setRadioChannel(tmp);
                        AbstractApp::sc->getRadio()->setChannel(tmp);
                        break;
                    case ADDRESS:
                        config->setRadioAddress(tmp);
                        break;
                }
                return;
            case Keypad::keyD:
                // Exit without saving
                return;
        }
    }
}

void RadioChannelAddressApp::executeChannel(void) 
{
    currentAction = CHANNEL;
    execute();
}

void RadioChannelAddressApp::executeAddress(void) 
{
    currentAction = ADDRESS;
    execute();
}

uint8_t RadioChannelAddressApp::increment(Keypad* keypad, Nokia_LCD* lcd, Headline* headline, uint8_t vMin, uint8_t vMax, uint8_t tmp, int8_t direction)
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

void RadioChannelAddressApp::update(Nokia_LCD* lcd, uint8_t value)
{
    lcd->setCursor(54, 3);
    lcd->print(StringAssets::space);
    lcd->print(StringAssets::space);
    lcd->print(StringAssets::space);
    lcd->setCursor(48, 3);

    lcd->print(value);
}