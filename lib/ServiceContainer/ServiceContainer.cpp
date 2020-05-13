#include <ServiceContainer.h>

ServiceContainer::ServiceContainer(ConfigStorage* _configStorage, Nokia_LCD* _lcd, Keypad* _keypad, uint8_t _buzzerPin)
{
    configStorage = _configStorage;
    lcd = _lcd;
    keypad = _keypad;
    buzzerPin = _buzzerPin;

    lcd->setBacklight(configStorage->isWithBacklight());
    lcd->begin();
    lcd->setContrast(configStorage->getContrast());
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