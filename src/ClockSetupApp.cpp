#include "ClockSetupApp.h"

AbstractApp::APPS ClockSetupApp::execute(void)
{
    Keypad *keypad = AbstractApp::sc->getKeypad();
    Headline *headline = AbstractApp::sc->getHeadline();
    Nokia_LCD *lcd = AbstractApp::sc->getLcd();

    lcd->clear(false);
    blinkLastUpdated = millis() - blinkRefreshMilis;
    currentComponent = HOUR;

    while (true)
    {
        headline->update();
        setComponentValue();
        drawComponents();

        if (!keypad->read())
        {
            continue;
        }

        switch (keypad->getKeypadSymbol())
        {
        case Keypad::keyB:
            increaseValue();
            break;
        case Keypad::keyC:
            decreaseValue();
            break;
        case Keypad::keyStar:
            nextComponent();
            break;
        case Keypad::keyHash:
            prevComponent();
            break;
        case Keypad::keyD: //Exit
            return AbstractApp::APPS::MAIN_MENU;
        }
    }
}

void ClockSetupApp::drawComponents(void)
{
    if (!isReachedTimer(blinkLastUpdated, blinkRefreshMilis))
    {
        return;
    }

    MenuRenderer *menuRenderer = AbstractApp::sc->getMenuRenderer();
    Nokia_LCD *lcd = AbstractApp::sc->getLcd();
    DS1307RTC *rtc = AbstractApp::sc->getRtc();

    tmElements_t tm;
    rtc->read(tm);

    // Clear area for values
    lcd->setCursor(0, 3);
    for (uint8_t i = 0; i < 14; i++)
    {
        lcd->print(StringAssets::space);
    }

    switch (currentComponent)
    {
    case HOUR:
    case MINUTE:
        menuRenderer->render_header(StringAssets::clockCtrlTime);
        lcd->setCursor(27, 3);
        lcd->setInverted(currentComponent == HOUR && selectComponent);
        printValue(tm.Hour);

        lcd->setInverted(false);
        lcd->print(StringAssets::colon);

        lcd->setInverted(currentComponent == MINUTE && selectComponent);
        printValue(tm.Minute);
        lcd->setInverted(false);
        break;
    case DAY:
    case MONTH:
    case YEAR:
        menuRenderer->render_header(StringAssets::clockCtrlDate);
        lcd->setCursor(12, 3);
        lcd->setInverted(currentComponent == DAY && selectComponent);
        printValue(tm.Day);

        lcd->setInverted(false);
        lcd->print(StringAssets::dot);

        lcd->setInverted(currentComponent == MONTH && selectComponent);
        printValue(tm.Month);

        lcd->setInverted(false);
        lcd->print(StringAssets::dot);

        lcd->setInverted(currentComponent == YEAR && selectComponent);
        printValue(yearOffset + tm.Year);
        lcd->setInverted(false);
        break;
    }

    blinkLastUpdated = millis();
    selectComponent = !selectComponent;
}

void ClockSetupApp::printValue(short int value)
{
    Nokia_LCD *lcd = AbstractApp::sc->getLcd();
    if (value < 10)
    {
        lcd->print(0);
    }
    lcd->print(value);
}

void ClockSetupApp::nextComponent(void)
{
    changeComponent(true);
}

void ClockSetupApp::prevComponent(void)
{
    changeComponent(false);
}

void ClockSetupApp::increaseValue(void)
{
    changeValue(true);
}

void ClockSetupApp::decreaseValue(void)
{
    changeValue(false);
}

void ClockSetupApp::changeComponent(bool increase)
{
    switch (currentComponent)
    {
    case HOUR:
        currentComponent = increase ? MINUTE : YEAR;
        break;
    case MINUTE:
        currentComponent = increase ? DAY : HOUR;
        break;
    case DAY:
        currentComponent = increase ? MONTH : MINUTE;
        break;
    case MONTH:
        currentComponent = increase ? YEAR : DAY;
        break;
    case YEAR:
        currentComponent = increase ? HOUR : MONTH;
        break;
    }

    blinkLastUpdated = millis() - blinkRefreshMilis;
}

void ClockSetupApp::changeValue(bool increase)
{
    DS1307RTC *rtc = AbstractApp::sc->getRtc();
    short int min, max;
    componentValue = increase ? componentValue += 1 : componentValue -= 1;
    switch (currentComponent)
    {
    case HOUR:
        min = 0;
        max = 23;
        break;
    case MINUTE:
        min = 0;
        max = 59;
        break;
    case DAY:
        min = 1;
        max = 31;
        break;
    case MONTH:
        min = 1;
        max = 12;
        break;
    case YEAR:
        min = 2020;
        max = 2050;
        break;
    }

    if (componentValue < min)
    {
        componentValue = max;
    }
    if (componentValue > max)
    {
        componentValue = min;
    }

    tmElements_t tm;
    rtc->read(tm);

    switch (currentComponent)
    {
    case HOUR:
        tm.Hour = componentValue;
        break;
    case MINUTE:
        tm.Minute = componentValue;
        break;
    case DAY:
        tm.Day = componentValue;
        break;
    case MONTH:
        tm.Month = componentValue;
        break;
    case YEAR:
        tm.Year = componentValue - yearOffset;
        break;
    }

    rtc->write(tm);
}

void ClockSetupApp::setComponentValue(void)
{
    DS1307RTC *rtc = AbstractApp::sc->getRtc();

    tmElements_t tm;
    rtc->read(tm);

    switch (currentComponent)
    {
    case HOUR:
        componentValue = tm.Hour;
        break;
    case MINUTE:
        componentValue = tm.Minute;
        break;
    case DAY:
        componentValue = tm.Day;
        break;
    case MONTH:
        componentValue = tm.Month;
        break;
    case YEAR:
        componentValue = yearOffset + tm.Year;
        break;
    default:
        componentValue = 0;
    }
}

bool ClockSetupApp::isReachedTimer(unsigned long lastUpdated, unsigned long refreshTimeMilis)
{
    unsigned long m = millis();
    return m < lastUpdated
        ? true
        : m - lastUpdated > refreshTimeMilis;
}