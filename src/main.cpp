#include <main.h>

//#define DEBUG true

const uint8_t keypadMoSiCSPin = 8;
const uint8_t keypadMiSoCSPin = 7; //CLK INH, Latch on the output is connected to the SS output.
const uint8_t buzzerPin = 6;

// 5110
const uint8_t nokiaDCPin = 4; // Data/Command select (D/C)
const uint8_t nokiaBLPin = 2; // Back light
const uint8_t nokiaCEPin = 3; // Chip Select
const uint8_t nokiaRSTPin = 5; // Reset
uint8_t nokiaContrast = 40;
bool nokiaBL = true;

Nokia_LCD lcd(SCK, MOSI, nokiaDCPin, nokiaCEPin, nokiaRSTPin);
Keypad keypad(keypadMoSiCSPin, keypadMiSoCSPin, buzzerPin, true);

void setup()
{
  pinMode(keypadMoSiCSPin, OUTPUT);
  pinMode(keypadMiSoCSPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(nokiaDCPin, OUTPUT);
  pinMode(nokiaCEPin, OUTPUT);
  pinMode(nokiaRSTPin, OUTPUT);
  pinMode(nokiaBLPin, OUTPUT);
  
  digitalWrite(nokiaBLPin, nokiaBL);
  lcd.begin();
  lcd.setContrast(nokiaContrast);
  lcd.clear(false);
  lcd.setCursor(76, 0);
  lcd.draw(LcdAssets::batteryFull, sizeof(LcdAssets::batteryFull) / sizeof(uint8_t), true);
  #if defined(DEBUG)
  Serial.begin(9600);
  #endif
}

void loop()
{
  if (!keypad.isKeypadUpdated()) {
    return;
  }
  handleKeypadSymbol(keypad.getKeypadSymbol());
}

void handleKeypadSymbol(uint8_t keypadSymbol)
{
  if (keypadSymbol != Keypad::keyMultiSymbol && keypadSymbol != Keypad::keyReleasedSymbol) {
    #if defined(DEBUG)
    Serial.print(int(keypadSymbol));
    #endif 

    lcd.clear(false);
    lcd.setCursor(0, 2);
    if (keypadSymbol < 10) {
      lcd.print(keypadSymbol);
      return;
    }

    switch (keypadSymbol) {
      case Keypad::keyB:
        nokiaContrast++;
        setContrast();
        
        break;
      case Keypad::keyC:
        nokiaContrast--;
        setContrast();
        
        break;
      case Keypad::keyStar:
        nokiaBL = !nokiaBL;
        digitalWrite(nokiaBLPin, nokiaBL);
        
        break;
      case Keypad::keyA:
        lcd.setInverted(true);
        
        break;
      case Keypad::keyD:
        lcd.setInverted(false);
        
        break;
      default:
        lcd.println("This site does not exist within the LDAP database");
        lcd.print(keypadSymbol);
    }
  }
}

void setContrast(void)
{
  lcd.clear();
  lcd.setContrast(nokiaContrast);
  lcd.print(" Contrast: ");
  lcd.print(nokiaContrast);
}