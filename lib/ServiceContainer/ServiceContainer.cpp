#include <ServiceContainer.h>

ServiceContainer::ServiceContainer(
    ConfigStorage *_configStorage,
    Nokia_LCD *_lcd,
    Keypad *_keypad,
    Headline *_headline,
    ByteLoader *_byteLoader32,
    ByteLoader *_byteLoader08,
    DS1307RTC *_rtc,
    TonePlayer *_tonePlayer,
    RF24 *_radio,
    MenuRenderer *_menuRenderer)
{
    configStorage = _configStorage;
    lcd = _lcd;
    keypad = _keypad;
    headline = _headline;
    byteLoader32 = _byteLoader32;
    byteLoader08 = _byteLoader08;
    tonePlayer = _tonePlayer;
    rtc = _rtc;
    radio = _radio;
    menuRenderer = _menuRenderer;
}

ConfigStorage *ServiceContainer::getConfigStorage(void)
{
    return configStorage;
}

Nokia_LCD *ServiceContainer::getLcd(void)
{
    return lcd;
}

Keypad *ServiceContainer::getKeypad(void)
{
    return keypad;
}

Headline *ServiceContainer::getHeadline(void)
{
    return headline;
}

MenuRenderer *ServiceContainer::getMenuRenderer(void)
{
    return menuRenderer;
}

ByteLoader *ServiceContainer::getByteLoader32(void)
{
    return byteLoader32;
}

ByteLoader *ServiceContainer::getByteLoader08(void)
{
    return byteLoader08;
}

DS1307RTC *ServiceContainer::getRtc(void)
{
    return rtc;
}

TonePlayer *ServiceContainer::getTonePlayer(void)
{
    return tonePlayer;
}

RF24 *ServiceContainer::getRadio(void)
{
    return radio;
}