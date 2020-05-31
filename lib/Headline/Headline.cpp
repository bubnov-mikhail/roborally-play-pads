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

    lastUpdated = millis();
    displayClockDots = !displayClockDots;
}

inline void Headline::updateRtc(void)
{
    tmElements_t tm;
    if (rtc->read(tm)) {
        lcd->setCursor(0, 0);
        printValue(tm.Hour);
        if (displayClockDots) {
            lcd->print(" ");
        } else {
            lcd->print(":");
        }
        printValue(tm.Minute);
    }
}

inline void Headline::updateBattery(void)
{
    // This is demo.
    lcd->setCursor(76, 0);
    lcd->draw(LcdAssets::batteryFull, sizeof(LcdAssets::batteryFull) / sizeof(uint8_t), true);
}

inline void Headline::printValue(uint8_t value)
{
    if (value < 10) {
        lcd->print(0);
    }
    lcd->print(value);
}
