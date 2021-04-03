#include <Keypad.h>
#include <Arduino.h>
#include <SPI.h>

Keypad::Keypad(uint8_t keypadMoSiCS, uint8_t keypadMiSoData, uint8_t keypadMiSoSck, uint8_t buzzerPin, bool beepOnClick)
{
  _keypadMoSiCS = keypadMoSiCS;
  _keypadMiSoData = keypadMiSoData;
  _keypadMiSoSck = keypadMiSoSck;
  _buzzerPin = buzzerPin;
  _beepOnClick = beepOnClick;
}

Keypad::Keypad(TonePlayer *tonePlayer, uint8_t keypadMoSiCS, uint8_t keypadMiSoData, uint8_t keypadMiSoSck, uint8_t buzzerPin, bool beepOnClick)
{
  _tonePlayer = tonePlayer;
  _keypadMoSiCS = keypadMoSiCS;
  _keypadMiSoData = keypadMiSoData;
  _keypadMiSoSck = keypadMiSoSck;
  _buzzerPin = buzzerPin;
  _beepOnClick = beepOnClick;
}

Keypad::Keypad(uint8_t keypadMoSiCS, uint8_t keypadMiSoData, uint8_t keypadMiSoSck)
{
  _keypadMoSiCS = keypadMoSiCS;
  _keypadMiSoData = keypadMiSoData;
  _keypadMiSoSck = keypadMiSoSck;
  _beepOnClick = false;
}

uint8_t Keypad::getKeypadSymbol(void)
{
  uint16_t code = keypadCode;
  if (code == 0x00)
  {
    return keyReleasedSymbol;
  }

  for (int i = 0; i < buttons; i++)
  {
    if (keyValues[i] == code)
    {
      return keySymbols[i];
    }
  }

#if defined(DEBUG)
  Serial.print(int(keypadSymbol));
#endif
  return keyMultiSymbol;
}

uint16_t Keypad::getKeypadCode(void)
{
  return keypadCode;
}

bool Keypad::read(void)
{
  if (keypadCode != 0x00 && millis() - debounceDelayMilis < keyLastUpdated)
  {
    return false;
  }

  uint16_t lastCode = keypadCode;
  keypadCode = 0x00;

  SPI.begin();
  uint8_t outputMask = 0x01;
  for (int i = 0; i < 4; i++)
  {
    keypadCode = keypadCode << 4;
    outputMask = outputMask << 1; //Ignore QA output;
    digitalWrite(_keypadMoSiCS, LOW);
    SPI.transfer(outputMask);
    digitalWrite(_keypadMoSiCS, HIGH);

    //Reload the lock on the input
    digitalWrite(_keypadMoSiCS, LOW);
    digitalWrite(_keypadMoSiCS, HIGH);
    uint8_t inputData = shiftIn(_keypadMiSoData, _keypadMiSoSck, MSBFIRST) >> 1;

    keypadCode = keypadCode | inputData;
  }

  // Turn off the keypad lines
  digitalWrite(_keypadMoSiCS, LOW);
  SPI.transfer(0x00);
  digitalWrite(_keypadMoSiCS, HIGH);
  SPI.end();

  bool updated = lastCode != keypadCode;
  if (updated && keypadCode > 0)
  {
    keyLastUpdated = millis();
    beepOnClick(keypadCode);
  }
  return updated;
}

void Keypad::begin(void)
{
  pinMode(_keypadMoSiCS, OUTPUT);
  pinMode(_keypadMiSoSck, OUTPUT);
  pinMode(_keypadMiSoData, INPUT);

  digitalWrite(_keypadMiSoSck, HIGH);
  digitalWrite(_keypadMiSoData, LOW);
  digitalWrite(_keypadMoSiCS, HIGH);
  read(); // Read to set all outputs to LOW, including LED outputs.
}

void Keypad::setBeepOnClick(bool beepOnClick)
{
  _beepOnClick = beepOnClick;
}

void Keypad::beepOnClick(uint16_t keypadCode)
{
  if (!_beepOnClick || !_buzzerPin)
  {
    return;
  }

  if (_tonePlayer == NULL)
  {
    TimerFreeTone(_buzzerPin, beepFreq, beepDelayMilis);
  }

  switch (getKeypadSymbol())
  {
  case keyStar:
    _tonePlayer->playTones(inButtonTones, inButtonTonesLength, false);
    return;
  case keyD:
    _tonePlayer->playTones(outButtonTones, outButtonTonesLength, false);
    return;
  default:
    TimerFreeTone(_buzzerPin, beepFreq, beepDelayMilis);
  }
}