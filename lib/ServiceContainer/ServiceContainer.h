#pragma once

#include <ConfigStorage.h>
#include <Keypad.h>
#include <Nokia_LCD.h>

class ServiceContainer {
    public:
        ServiceContainer(ConfigStorage* _configStorage, Nokia_LCD* _lcd, Keypad* _keypad, uint8_t _buzzerPin);
        const ConfigStorage* getConfigStorage(void);
        const Nokia_LCD* getLcd(void);
        const Keypad* getKeypad(void);
    private:
        const ConfigStorage* configStorage;
        const Nokia_LCD* lcd;
        const Keypad* keypad;
        uint8_t buzzerPin;
};