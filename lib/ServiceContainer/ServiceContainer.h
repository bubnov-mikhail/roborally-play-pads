#pragma once

#include <ConfigStorage.h>
#include <Keypad.h>
#include <Nokia_LCD.h>
#include <MenuRenderer.h>
#include <Headline.h>
#include <BitmapLoader.h>

class ServiceContainer {
    public:
        ServiceContainer(ConfigStorage* _configStorage, Nokia_LCD* _lcd, Keypad* _keypad, Headline* _headline, BitmapLoader* _bitmapLoader, uint8_t _buzzerPin);
        ConfigStorage* getConfigStorage(void);
        Nokia_LCD* getLcd(void);
        Keypad* getKeypad(void);
        Headline* getHeadline(void);
        MenuRenderer* getMenuRenderer(void);
        BitmapLoader* getBitmapLoader(void);
    private:
        ConfigStorage* configStorage;
        MenuRenderer* menuRenderer;
        Nokia_LCD* lcd;
        Keypad* keypad;
        Headline* headline;
        BitmapLoader* bitmapLoader;
        uint8_t buzzerPin;
};