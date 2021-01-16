#include <Headline.h>

Headline::Headline(ConfigStorage* _configStorage, Nokia_LCD* _lcd, DS1307RTC* _rtc, uint8_t _pinVoltageRead)
{
    configStorage = _configStorage;
    lcd = _lcd;
    rtc = _rtc;
    pinVoltageRead = _pinVoltageRead;
    lastUpdated = millis();
}

void Headline::update(bool forceUpdate /*= false*/)
{
    if (millis() - lastUpdated < refreshTimeMilis && !forceUpdate) {
        return;
    }

    updateRtc();
    updateBattery();
    updateRadio();

    lastUpdated = millis();
    displayClockDots = !displayClockDots;
}

inline void Headline::updateRtc(void)
{
    tmElements_t tm;
    if (rtc->read(tm)) {
        lcd->setCursor(0, 0);
        printValue(tm.Hour);
        lcd->setCursor(lcd->getCursorX() + 1, 0);
        if (displayClockDots) {
            lcd->draw(LcdAssets::smallInts[11], 3, true);
        } else {
            lcd->draw(LcdAssets::smallInts[10], 3, true);
        }
        printValue(tm.Minute);
    }
}

inline void Headline::updateRadio(void)
{
    lcd->setCursor(55, 0);
    lcd->draw(LcdAssets::radioSignal, 5, true);

    if (!configStorage->isRadioConnected()) {
        lcd->setCursor(lcd->getCursorX() + 1, 0);
        lcd->draw(LcdAssets::radioNotAvailable, 3, true);
        return;
    }

    printDigits(configStorage->getRadioChannel());
}

inline void Headline::updateBattery(void)
{
    int voltage = analogRead(pinVoltageRead);    
    lcd->setCursor(76, 0);
    if (voltage < 682 && displayClockDots) {
        lcd->draw(LcdAssets::batteryBlank, 8, true);
        return;
    }
    lcd->draw((voltage >= 780) ? LcdAssets::batteryFull : (voltage >= 682 ? LcdAssets::batteryHalf : LcdAssets::batteryLow), 8, true);
}

void Headline::printValue(uint8_t value)
{
    if (value < 10) {
        printDigit(0);
        printDigit(value);
        
        return;
    }
    uint8_t tens = value / 10;
    printDigit(tens);
    printDigit(value - tens*10);
}

void Headline::printDigit(uint8_t value)
{
    lcd->setCursor(lcd->getCursorX() + 1, 0);
    lcd->draw(LcdAssets::smallInts[value], 3, true);
}

void Headline::printDigits(uint8_t value)
{
    if(value >= 10) {
       printDigits(value / 10);
    }

    int digit = value % 10;
    lcd->setCursor(lcd->getCursorX() + 1, 0);
    lcd->draw(LcdAssets::smallInts[digit], 3, true);
}
