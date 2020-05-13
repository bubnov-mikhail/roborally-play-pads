#include <ConfigStorage.h>
#include <EEPROM.h>

ConfigStorage::ConfigStorage(void)
{
    initAddresses();
    
    EEPROM.get(withBacklightAddress, withBacklight);
    EEPROM.get(withSoundsAddress, withSounds);
    EEPROM.get(contrastAddress, contrast);
}

ConfigStorage::ConfigStorage(bool _withBacklight, bool _withSounds, uint8_t _contrast)
{
    initAddresses();
    withBacklight = _withBacklight;
    withSounds = _withSounds;
    contrast = _contrast;
    setWithBacklight(withBacklight);
    setWithSounds(withSounds);
    setContrast(contrast);
}

bool ConfigStorage::isWithBacklight()
{
    return withBacklight;
}

bool ConfigStorage::isWithSounds()
{
    return withSounds;
}

uint8_t ConfigStorage::getContrast()
{
    return contrast;
}

void ConfigStorage::setWithBacklight(bool value)
{
    withBacklight = value;
    EEPROM.put(withBacklightAddress, withBacklight);

}

void ConfigStorage::setWithSounds(bool value)
{
    withSounds = value;
    EEPROM.put(withSoundsAddress, withSounds);
}

void ConfigStorage::setContrast(uint8_t value)
{
    contrast = value;
    EEPROM.put(contrastAddress, contrast);
}

void ConfigStorage::initAddresses(void)
{
    static bool EEMEM _withBacklightAddress;
    static bool EEMEM _withSoundsAddress;
    static uint8_t EEMEM _contrastAddress;

    withBacklightAddress = (int)&_withBacklightAddress;
    withSoundsAddress = (int)&_withSoundsAddress;
    contrastAddress = (int)&_contrastAddress;
}