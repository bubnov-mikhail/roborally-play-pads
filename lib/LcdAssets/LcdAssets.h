#pragma once
// If we are not building for AVR architectures ignore PROGMEM
#if defined(__AVR__)
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif

namespace LcdAssets {
    const unsigned char batteryFull[] PROGMEM = {0x7e, 0x7d, 0x7d, 0x7d, 0x7e};
    const unsigned char batteryHalf[] PROGMEM = {0x7e, 0x71, 0x71, 0x71, 0x7e};
    const unsigned char batteryLow[] PROGMEM = {0x7e, 0x41, 0x41, 0x41, 0x7e};
    const unsigned char progressBarSideBorder PROGMEM = 0xff;
    const unsigned char progressBarEmptyBody PROGMEM = 0x81;
    const unsigned char progressBarFilledBody PROGMEM = 0xbd;
}