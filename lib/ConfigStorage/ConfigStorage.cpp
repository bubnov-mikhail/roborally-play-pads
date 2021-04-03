#include <ConfigStorage.h>
#include <EEPROM.h>

ConfigStorage::ConfigStorage(void)
{
    initAddresses();

    EEPROM.get(withBacklightAddress, withBacklight);
    EEPROM.get(withSoundsAddress, withSounds);
    EEPROM.get(contrastAddress, contrast);
    EEPROM.get(radioChannelAddress, radioChannel);
    EEPROM.get(radioLevelAddress, radioLevel);
}

ConfigStorage::ConfigStorage(bool _withBacklight, bool _withSounds, uint8_t _contrast, uint8_t _radioChannel, uint8_t _radioLevel)
{
    initAddresses();
    withBacklight = _withBacklight;
    withSounds = _withSounds;
    contrast = _contrast;
    radioChannel = _radioChannel;
    radioLevel = _radioLevel;
    setWithBacklight(withBacklight);
    setWithSounds(withSounds);
    setContrast(contrast);
    setRadioChannel(radioChannel);
    setRadioLevel(radioLevel);
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

uint8_t ConfigStorage::getRadioChannel()
{
    return radioChannel;
}

void ConfigStorage::setRadioChannel(uint8_t value)
{
    radioChannel = value;
    EEPROM.put(radioChannelAddress, radioChannel);
}

uint8_t ConfigStorage::getRadioLevel()
{
    return radioLevel;
}

void ConfigStorage::setRadioLevel(uint8_t value)
{
    radioLevel = value;
    EEPROM.put(radioLevelAddress, radioLevel);
}

void ConfigStorage::setRadioConnected(bool value)
{
    radioConnected = value;
}

bool ConfigStorage::isRadioConnected()
{
    return radioConnected;
}

void ConfigStorage::initAddresses(void)
{
    static bool EEMEM _withBacklightAddress;
    static bool EEMEM _withSoundsAddress;
    static uint8_t EEMEM _contrastAddress;
    static uint8_t EEMEM _radioChannelAddress;
    static uint8_t EEMEM _radioLevelAddress;

    withBacklightAddress = (int)&_withBacklightAddress;
    withSoundsAddress = (int)&_withSoundsAddress;
    contrastAddress = (int)&_contrastAddress;
    radioChannelAddress = (int)&_radioChannelAddress;
    radioLevelAddress = (int)&_radioLevelAddress;
}