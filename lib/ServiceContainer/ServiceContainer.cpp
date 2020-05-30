#include <ServiceContainer.h>

ServiceContainer::ServiceContainer(ConfigStorage* _configStorage, Nokia_LCD* _lcd, Keypad* _keypad, Headline* _headline, uint8_t _buzzerPin)
{
    configStorage = _configStorage;
    lcd = _lcd;
    keypad = _keypad;
    headline = _headline;
    buzzerPin = _buzzerPin;

    lcd->setBacklight(configStorage->isWithBacklight());
    lcd->begin();
    lcd->setContrast(configStorage->getContrast());
    menuRenderer = new MenuRenderer(lcd);
}

ConfigStorage* ServiceContainer::getConfigStorage(void)
{
    return configStorage;
}

Nokia_LCD* ServiceContainer::getLcd(void)
{
    return lcd;
}

Keypad* ServiceContainer::getKeypad(void)
{
    return keypad;
}

Headline* ServiceContainer::getHeadline(void)
{
    return headline;
}

MenuRenderer* ServiceContainer::getMenuRenderer(void)
{
    return menuRenderer;
}