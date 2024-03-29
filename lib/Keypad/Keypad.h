// ---------------------------------------------------------------------------
// Keypad Library - v1.0
//
// AUTHOR/LICENSE:
// Created by Mihail Bubnov- bubnov.mihail@gmail.com
// Copyright 2020 License: GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
//
// LINKS:
// Project home: https://github.com/bubnov-mikhail/roborally-play-pads
//
// This library is using SPI to read a 4x4 Keypad

#pragma once

#include <TimerFreeTone.h>
#include <TonePlayer.h>

class Keypad
{
public:
    static const uint8_t key0 = 0x00;              //0
    static const uint8_t key1 = 0x01;              //1
    static const uint8_t key2 = 0x02;              //2
    static const uint8_t key3 = 0x03;              //3
    static const uint8_t key4 = 0x04;              //4
    static const uint8_t key5 = 0x05;              //5
    static const uint8_t key6 = 0x06;              //6
    static const uint8_t key7 = 0x07;              //7
    static const uint8_t key8 = 0x08;              //8
    static const uint8_t key9 = 0x09;              //9
    static const uint8_t keyA = 0x0A;              //10
    static const uint8_t keyB = 0x0B;              //11
    static const uint8_t keyC = 0x0C;              //12
    static const uint8_t keyD = 0x0D;              //13
    static const uint8_t keyStar = 0x0E;           //14
    static const uint8_t keyHash = 0x0F;           //15
    static const uint8_t keyMultiSymbol = 0x10;    //16 //Psevdo symbol to indicate that there are more then one key pressed
    static const uint8_t keyReleasedSymbol = 0x11; //17 Psevdo symbol to indicate that no key is pressed
    Keypad(uint8_t keypadMoSiCS, uint8_t keypadMiSoData, uint8_t keypadMiSoSck, uint8_t buzzerPin, bool beepOnClick);
    Keypad(TonePlayer *tonePlayer, uint8_t keypadMoSiCS, uint8_t keypadMiSoData, uint8_t keypadMiSoSck, uint8_t buzzerPin, bool beepOnClick);
    Keypad(uint8_t keypadMoSiCS, uint8_t keypadMiSoData, uint8_t keypadMiSoSck);
    uint16_t getKeypadCode(void);
    uint8_t getKeypadSymbol(void);
    bool read(void);
    void begin(void);
    void setBeepOnClick(bool beepOnClick);

private:
    uint16_t keypadCode = 0x00;
    uint16_t _keypadMoSiCS, _keypadMiSoData, _keypadMiSoSck, _buzzerPin;
    static const uint8_t buttons = 16;
    unsigned long keyLastUpdated = 0;
    TonePlayer *_tonePlayer;
    static const unsigned short int debounceDelayMilis = 20; //ms
    static const unsigned int beepDelayMilis = 40;           //ms
    static const unsigned long beepFreq = 450;
    const uint8_t keySymbols[buttons] = {
        key1, key2, key3, keyA,
        key4, key5, key6, keyB,
        key7, key8, key9, keyC,
        keyStar, key0, keyHash, keyD};
    const uint16_t keyValues[buttons] = {
        0x1000,
        0x100,
        0x10,
        0x01,
        0x2000,
        0x200,
        0x20,
        0x02,
        0x4000,
        0x400,
        0x40,
        0x04,
        0x8000,
        0x800,
        0x80,
        0x08,

    };
    const uint8_t inButtonTonesLength = 4;
    TonePlayer::Tone inButtonTones[4] = {
        {1024, 70},
        {520, 60},
        {1024, 70},
        {580, 70},
    };
    const uint8_t outButtonTonesLength = 4;
    TonePlayer::Tone outButtonTones[4] = {
        {700, 70},
        {360, 60},
        {700, 70},
        {360, 70},
    };
    bool _beepOnClick;
    void beepOnClick(uint16_t keypadCode);
};