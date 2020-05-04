#include <main.h>

#define DEBUG true

const uint8_t mosiPin = 8;
const uint8_t misoPin = 7; //CLK INH, Latch on the output is connected to the SS output.
const uint8_t buzzerPin = 6;

Keypad keypad(mosiPin, misoPin, buzzerPin, true);

void setup()
{
  pinMode(mosiPin, OUTPUT);
  pinMode(misoPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  #if defined(DEBUG)
  Serial.begin(9600);
  #endif
}

void loop()
{
  if (!keypad.isKeypadUpdated()) {
    return;
  }
  uint8_t keypadSymbol = keypad.getKeypadSymbol();
  if (keypadSymbol != Keypad::keyMultiSymbol && keypadSymbol != Keypad::keyReleasedSymbol) {
    #if defined(DEBUG)
    Serial.print(int(keypadSymbol));
    #endif 
  }
}