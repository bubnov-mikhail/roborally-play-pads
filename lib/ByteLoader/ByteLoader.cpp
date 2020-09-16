#include <ByteLoader.h>

ByteLoader::ByteLoader(Eeprom24C* _eeprom24c)
{
    eeprom24c = _eeprom24c;
}

const unsigned char* ByteLoader::loadBitmap(unsigned char* bitmap, unsigned int address, unsigned short int length)
{
    for (unsigned int i = 0; i < length; i++) {
        bitmap[i] = eeprom24c->read_1_byte(address + i);
    }
    
    return bitmap;
}

byte ByteLoader::loadByte(unsigned int address)
{
    return eeprom24c->read_1_byte(address);
}

const unsigned char* ByteLoader::loadByteToPosition(unsigned char* bitmap, unsigned int address, unsigned int position)
{
    bitmap[position] = eeprom24c->read_1_byte(address);

    return bitmap;
}