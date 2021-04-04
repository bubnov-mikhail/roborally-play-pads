#include "RadioChannelApp.h"

AbstractApp::APPS RadioChannelApp::execute(void)
{
    Nokia_LCD *lcd = AbstractApp::sc->getLcd();
    Keypad *keypad = AbstractApp::sc->getKeypad();
    ConfigStorage *config = AbstractApp::sc->getConfigStorage();
    MenuRenderer *menuRenderer = AbstractApp::sc->getMenuRenderer();
    Headline *headline = AbstractApp::sc->getHeadline();
    uint8_t tmp;

    lcd->clear(false);
    tmp = config->getRadioChannel();
    menuRenderer->render_header(StringAssets::radioChannel);
    lcd->setCursor(0, 3);
    lcd->print(StringAssets::radioChannel);
    lcd->print(StringAssets::colon);
    headline->update(true);
    update(lcd, tmp);

    while (true)
    {
        headline->update();
        if (!keypad->read())
        {
            continue;
        }
        switch (keypad->getKeypadSymbol())
        {
        case Keypad::keyB:
            tmp = increment(keypad, lcd, headline, vMin, vMax, tmp, 1);
            break;
        case Keypad::keyC:
            tmp = increment(keypad, lcd, headline, vMin, vMax, tmp, -1);
            break;
        case Keypad::keyHash:
            // Reset to config
            tmp = config->getRadioChannel();
            update(lcd, tmp);
            break;
        case Keypad::keyStar:
            // Save and exit
            config->setRadioChannel(tmp);
            AbstractApp::sc->getRadio()->setChannel(tmp);
            return AbstractApp::APPS::MAIN_MENU;
        case Keypad::keyD:
            // Exit without saving
            return AbstractApp::APPS::MAIN_MENU;
        }
    }
}

uint8_t RadioChannelApp::increment(Keypad *keypad, Nokia_LCD *lcd, Headline *headline, uint8_t vMin, uint8_t vMax, uint8_t tmp, int8_t direction)
{
    tmp = min(vMax, max(vMin, tmp + direction));
    update(lcd, tmp);
    unsigned long longDelay = millis();
    unsigned long shortDelay;

    while (true)
    {
        headline->update();
        if (keypad->read())
        {
            return tmp;
        }

        if (!isReachedTimer(longDelay, 300))
        {
            continue;
        }
        else if (longDelay > 0)
        {
            longDelay = 0;
            shortDelay = millis();
        }

        if (!isReachedTimer(shortDelay, 50))
        {
            continue;
        }
        shortDelay = millis();
        tmp = min(vMax, max(vMin, tmp + direction));
        update(lcd, tmp);
    }

    return tmp;
}

void RadioChannelApp::update(Nokia_LCD *lcd, uint8_t value)
{
    lcd->setCursor(54, 3);
    lcd->print(StringAssets::space);
    lcd->print(StringAssets::space);
    lcd->print(StringAssets::space);
    lcd->setCursor(48, 3);

    lcd->print(value);
}

bool RadioChannelApp::isReachedTimer(unsigned long lastUpdated, unsigned long refreshTimeMilis)
{
    unsigned long m = millis();
    return m < lastUpdated
        ? true
        : m - lastUpdated > refreshTimeMilis;
}