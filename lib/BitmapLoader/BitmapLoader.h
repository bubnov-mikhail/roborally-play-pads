#pragma once

#include <Eeprom24Cxx.h>

class BitmapLoader {
    public:
        BitmapLoader(Eeprom24C* _eeprom24c);
        const unsigned char* loadBitmap(unsigned char* bitmap, unsigned int address, unsigned short int length);
        const unsigned char* loadByteToPosition(unsigned char* bitmap, unsigned int address, unsigned int position);
    protected:
        Eeprom24C* eeprom24c;
};