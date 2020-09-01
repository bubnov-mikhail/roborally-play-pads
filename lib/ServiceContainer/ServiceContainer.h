#pragma once

#include <ConfigStorage.h>
#include <Keypad.h>
#include <Nokia_LCD.h>
#include <MenuRenderer.h>
#include <Headline.h>
#include <BitmapLoader.h>
#include <DS1307RTC.h>

class ServiceContainer {
    public:
        ServiceContainer(
            ConfigStorage* _configStorage, 
            Nokia_LCD* _lcd, 
            Keypad* _keypad, 
            Headline* _headline, 
            BitmapLoader* _bitmapLoader, 
            DS1307RTC* _rtc, 
            uint8_t _buzzerPin
        );
        ConfigStorage* getConfigStorage(void);
        Nokia_LCD* getLcd(void);
        Keypad* getKeypad(void);
        Headline* getHeadline(void);
        MenuRenderer* getMenuRenderer(void);
        BitmapLoader* getBitmapLoader(void);
        DS1307RTC* ServiceContainer::getRtc(void);
    private:
        ConfigStorage* configStorage;
        MenuRenderer* menuRenderer;
        Nokia_LCD* lcd;
        Keypad* keypad;
        Headline* headline;
        BitmapLoader* bitmapLoader;
        DS1307RTC* rtc;
        uint8_t buzzerPin;
};