#pragma once

#include <ConfigStorage.h>
#include <Keypad.h>
#include <Nokia_LCD.h>
#include <MenuRenderer.h>

class ServiceContainer {
    public:
        ServiceContainer(ConfigStorage* _configStorage, Nokia_LCD* _lcd, Keypad* _keypad, uint8_t _buzzerPin);
        ConfigStorage* getConfigStorage(void);
        Nokia_LCD* getLcd(void);
        Keypad* getKeypad(void);
        MenuRenderer* getMenuRenderer(void);
    private:
        ConfigStorage* configStorage;
        MenuRenderer* menuRenderer;
        Nokia_LCD* lcd;
        Keypad* keypad;
        uint8_t buzzerPin;
};