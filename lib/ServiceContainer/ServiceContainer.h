#pragma once

#include <ConfigStorage.h>
#include <Keypad.h>
#include <Nokia_LCD.h>
#include <MenuRenderer.h>
#include <Headline.h>
#include <ByteLoader.h>
#include <DS1307RTC.h>
#include <TonePlayer.h>
#include "RF24.h"

class ServiceContainer
{
public:
    ServiceContainer(
        ConfigStorage *_configStorage,
        Nokia_LCD *_lcd,
        Keypad *_keypad,
        Headline *_headline,
        ByteLoader *_ByteLoader32,
        ByteLoader *_ByteLoader08,
        DS1307RTC *_rtc,
        TonePlayer *_tonePlayer,
        RF24 *_radio,
        MenuRenderer *_menuRenderer);
    ConfigStorage *getConfigStorage(void);
    Nokia_LCD *getLcd(void);
    Keypad *getKeypad(void);
    Headline *getHeadline(void);
    MenuRenderer *getMenuRenderer(void);
    ByteLoader *getByteLoader32(void);
    ByteLoader *getByteLoader08(void);
    TonePlayer *getTonePlayer(void);
    DS1307RTC *getRtc(void);
    RF24 *getRadio(void);

private:
    ConfigStorage *configStorage;
    MenuRenderer *menuRenderer;
    Nokia_LCD *lcd;
    Keypad *keypad;
    Headline *headline;
    ByteLoader *byteLoader32;
    ByteLoader *byteLoader08;
    DS1307RTC *rtc;
    TonePlayer *tonePlayer;
    RF24 *radio;
};