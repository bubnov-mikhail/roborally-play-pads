#pragma once
// If we are not building for AVR architectures ignore PROGMEM
#if defined(__AVR__)
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif

namespace LcdAssets {
    static const uint8_t displayWidth = 84;
    static const uint8_t charWidth = 6;
    // Gfx assets stored on Eeprom24C08
    const unsigned int roborallyMainScreenAddress = 0;
    const unsigned short int roborallyMainScreenLength = 420;

    // Bitmaps
    const unsigned char batteryFull[] PROGMEM = {0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x11, 0x0e};
    const unsigned char batteryHalf[] PROGMEM = {0x1f, 0x1f, 0x1f, 0x1f, 0x11, 0x11, 0x11, 0x0e};
    const unsigned char batteryLow[] PROGMEM = {0x1f, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0e};
    const unsigned char batteryBlank[] PROGMEM = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    const unsigned char progressBarSideBorder[] PROGMEM = {0xff};
    const unsigned char progressBarEmptyBody[] PROGMEM = {0x81};
    const unsigned char progressBarFilledBody[] PROGMEM = {0xbd};
    const unsigned char menuHeader[] PROGMEM = {0xf3, 0x79, 0x3c, 0x9e, 0xcf, 0xe7};
    const unsigned char menuItem[] PROGMEM = {0xff, 0x00, 0x00, 0x00};
    const unsigned char menuItemSelected[] PROGMEM = {0xff, 0xff, 0xff, 0x00};
    const unsigned char noise[] = {0x55, 0x96, 0x4b, 0xac, 0x52, 0x4d};
    const unsigned char roborallyRoundEmpty[] PROGMEM = {0x54, 0x54};
    const unsigned char roborallyRoundFilled[] PROGMEM = {0x57, 0x57};
    const unsigned char smallInts[][3] PROGMEM = {
        {0x3e, 0x22, 0x3e}, // 0
        {0x00, 0x02, 0x3e}, // 1
        {0x3a, 0x2a, 0x2e}, // 2
        {0x2a, 0x2a, 0x3e}, // 3
        {0x0e, 0x08, 0x3e}, // 4
        {0x2e, 0x2a, 0x3a}, // 5
        {0x3e, 0x2a, 0x3a}, // 6
        {0x02, 0x02, 0x3e}, // 7
        {0x3e, 0x2a, 0x3e}, // 8
        {0x2e, 0x2a, 0x3e}, // 9
        {0x00, 0x14, 0x00}, // :
        {0x00, 0x00, 0x00}, // empty space
    };
}