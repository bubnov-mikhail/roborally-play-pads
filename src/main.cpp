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
Nokia_LCD lcd(SCK, MOSI, PIN_NOKIA_DC, PIN_NOKIA_CE, PIN_NOKIA_RST, PIN_NOKIA_BL);
Keypad keypad(PIN_KEYPAD_MOSI_CS, PIN_KEYPAD_MISO_CS, PIN_BUZZER, configStorage.isWithSounds());

ServiceContainer serviceContainer(&configStorage, &lcd, &keypad, PIN_BUZZER);

void setup()
{
  pinMode(PIN_KEYPAD_MOSI_CS, OUTPUT);
  pinMode(PIN_KEYPAD_MISO_CS, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(PIN_NOKIA_DC, OUTPUT);
  pinMode(PIN_NOKIA_CE, OUTPUT);
  pinMode(PIN_NOKIA_RST, OUTPUT);
  pinMode(PIN_NOKIA_BL, OUTPUT);
  
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