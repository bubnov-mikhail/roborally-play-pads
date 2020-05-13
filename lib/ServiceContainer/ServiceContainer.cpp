#include <ServiceContainer.h>

ServiceContainer::ServiceContainer(ConfigStorage* _configStorage, Nokia_LCD* _lcd, Keypad* _keypad)
{
    configStorage = _configStorage;
    lcd = _lcd;
    keypad = _keypad;

    _lcd->setBacklight(_configStorage->isWithBacklight());
    _lcd->begin();
    _lcd->setContrast(_configStorage->getContrast());
}

const ConfigStorage* ServiceContainer::getConfigStorage(void)
{
    return configStorage;
}

const Nokia_LCD* ServiceContainer::getLcd(void)
{
    return lcd;
}

const Keypad* ServiceContainer::getKeypad(void)
{
    return keypad;
}