#include <main.h>

//#define DEBUG true
//#define SET_CONFIG_DEFAULTS true
uint8_t nokiaContrast = 50;

/**
 * Init services
 */

#if defined(SET_CONFIG_DEFAULTS)
  ConfigStorage configStorage(true, true, 50);
#else
  ConfigStorage configStorage;
#endif
Nokia_LCD lcd(SCK, MOSI, nokiaDCPin, nokiaCEPin, nokiaRSTPin, nokiaBLPin);
Keypad keypad(keypadMoSiCSPin, keypadMiSoCSPin, buzzerPin, configStorage.isWithSounds());

ServiceContainer serviceContainer(&configStorage, &lcd, &keypad);

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
        configStorage.setWithBacklight(!configStorage.isWithBacklight());
        lcd.setBacklight(configStorage.isWithBacklight());
        
        break;
      case Keypad::keyA:
        lcd.setInverted(true);
        
        break;
      case Keypad::keyD:
        lcd.setInverted(false);
        
        break;
      case Keypad::keyHash:
        configStorage.setWithSounds(!configStorage.isWithSounds());
        keypad.setBeepOnClick(configStorage.isWithSounds());
        break;
      default:
        lcd.setCursor(0, 0);
        lcd.setInverted(true);
        lcd.println("This is an inverted text");
        lcd.setInverted(false);
        lcd.println("This is a normal text.");
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