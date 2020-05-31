#include <Headline.h>

Headline::Headline(ConfigStorage* _configStorage, Nokia_LCD* _lcd, uint8_t _pinVoltageRead)
{
    configStorage = _configStorage;
    lcd = _lcd;
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
    // This is demo. RTC will provide real data here
    uint8_t hours = 23;
    uint8_t minutes = 5;

    lcd->setCursor(0, 0);
    lcd->print("12");
    if (displayClockDots) {
        lcd->print(" ");
    } else {
        lcd->print(":");
    }
    lcd->print("32");
}

inline void Headline::updateBattery(void)
{
    // This is demo.
    lcd->setCursor(76, 0);
    lcd->draw(LcdAssets::batteryFull, sizeof(LcdAssets::batteryFull) / sizeof(uint8_t), true);
}
