#pragma once
// If we are not building for AVR architectures ignore PROGMEM
#if defined(__AVR__)
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif

namespace LcdAssets {
    const unsigned char batteryFull[] PROGMEM = {0xfe, 0xfb, 0xfb, 0xfb, 0xfe};
    const unsigned char batteryHalf[] PROGMEM = {0xfe, 0xe3, 0xe3, 0xe3, 0xfe};
    const unsigned char batteryLow[] PROGMEM = {0xfe, 0x83, 0x83, 0x83, 0xfe};
}