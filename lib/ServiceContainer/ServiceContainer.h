#pragma once

#include <ConfigStorage.h>
#include <Keypad.h>
#include <Nokia_LCD.h>
#include <MenuRenderer.h>
#include <Headline.h>
#include <ByteLoader.h>
#include <DS1307RTC.h>

class ServiceContainer {
    public:
        ServiceContainer(
            ConfigStorage* _configStorage, 
            Nokia_LCD* _lcd, 
            Keypad* _keypad, 
            Headline* _headline, 
            ByteLoader* _ByteLoader32, 
            ByteLoader* _ByteLoader08, 
            DS1307RTC* _rtc, 
            uint8_t _buzzerPin
        );
        ConfigStorage* getConfigStorage(void);
        Nokia_LCD* getLcd(void);
        Keypad* getKeypad(void);
        Headline* getHeadline(void);
        MenuRenderer* getMenuRenderer(void);
        ByteLoader* getByteLoader32(void);
        ByteLoader* getByteLoader08(void);
        DS1307RTC* getRtc(void);
    private:
        ConfigStorage* configStorage;
        MenuRenderer* menuRenderer;
        Nokia_LCD* lcd;
        Keypad* keypad;
        Headline* headline;
        ByteLoader* byteLoader32;
        ByteLoader* byteLoader08;
        DS1307RTC* rtc;
        uint8_t buzzerPin;
};