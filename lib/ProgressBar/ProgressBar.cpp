#include <ProgressBar.h>

ProgressBar::ProgressBar(Nokia_LCD *_lcd, uint8_t _min, uint8_t _max, uint8_t _lcdY, bool _riseOnly)
{
    lcd = _lcd;
    min = _min;
    lastValue = _min;
    lcdY = _lcdY;
    max = _max;
    riseOnly = _riseOnly;
}

void ProgressBar::reset()
{
    lastValue = min;
    initRequired = true;
}

void ProgressBar::render(uint8_t percent)
{
    if (!riseOnly || initRequired)
    {
        lcd->setCursor(min - 2, lcdY);
        lcd->draw(LcdAssets::progressBarSideBorder, 1, true);
        lcd->draw(LcdAssets::progressBarEmptyBody, 1, true);
    }

    lcd->setCursor(lastValue, lcdY);
    uint8_t value = (max - min) * percent / 100 + min;
    for (int i = lastValue; i < max; i++)
    {
        lcd->draw(i > value ? LcdAssets::progressBarEmptyBody : LcdAssets::progressBarFilledBody, 1, true);
    }

    if (riseOnly)
    {
        lastValue = value;
    }

    if (!riseOnly || initRequired)
    {
        lcd->draw(LcdAssets::progressBarEmptyBody, 1, true);
        lcd->draw(LcdAssets::progressBarSideBorder, 1, true);
    }

    initRequired = false;
}