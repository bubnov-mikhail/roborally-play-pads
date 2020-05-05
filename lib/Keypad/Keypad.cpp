#include <Keypad.h>

Keypad::Keypad(uint8_t keypadMoSiCS, uint8_t keypadMiSoCS, uint8_t buzzerPin, bool beepOnClick)
{
    _keypadMoSiCS = keypadMoSiCS;
    _keypadMiSoCS = keypadMiSoCS;
    _buzzerPin = buzzerPin;
    _beepOnClick = beepOnClick;
}

Keypad::Keypad(uint8_t keypadMoSiCS, uint8_t keypadMiSoCS)
{
    _keypadMoSiCS = keypadMoSiCS;
    _keypadMiSoCS = keypadMiSoCS;
    _beepOnClick = false;
}

uint8_t Keypad::getKeypadSymbol(void)
{
    uint16_t code = keypadCode;
    if (code == 0x00) {
      return keyReleasedSymbol;
    }

    for (int i=0; i < buttons; i++) {
      if (keyValues[i] == code) {
        return keySymbols[i];
      }
    }

    return keyMultiSymbol;
}

uint16_t Keypad::getKeypadCode(void)
{
  return keypadCode;
}

bool Keypad::isKeypadUpdated(void)
{
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

    digitalWrite(_keypadMiSoCS, LOW);
    uint8_t inputData = SPI.transfer(0x00);
    digitalWrite(_keypadMiSoCS, HIGH);
    
    keypadCode = keypadCode | inputData;
  }
  digitalWrite(_keypadMoSiCS, LOW);
  SPI.transfer(0x00);
  digitalWrite(_keypadMoSiCS, HIGH);
  SPI.end();

  bool updated = lastCode != keypadCode;
  if (updated && keypadCode > 0) {
    beepOnClick();
  }
  return updated;
}

void Keypad::setBeepOnClick(bool beepOnClick)
{
    _beepOnClick = beepOnClick;
}

void Keypad::beepOnClick(void)
{
    if (_beepOnClick && _buzzerPin) {
        TimerFreeTone(_buzzerPin, 450, 40);
    }
}