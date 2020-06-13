#include <main.h>

//#define DEBUG true
//#define SET_CONFIG_DEFAULTS true
//#define SET_GFX_ASSETS true
#define Eeprom24C08_address 0x50
#if defined(SET_GFX_ASSETS)
  #include "GfxAssets.h"
#endif
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
Headline headline(&configStorage, &lcd, &RTC, PIN_VOLTAGE_READ);
Eeprom24C eeprom24c08(8, Eeprom24C08_address);
BitmapLoader bitmapLoader(&eeprom24c08);
ServiceContainer serviceContainer(&configStorage, &lcd, &keypad, &headline, &bitmapLoader, PIN_BUZZER);
ServiceContainer *AbstractApp::sc = &serviceContainer;

void setup()
{
  #if defined(DEBUG)
    Serial.begin(9600);
  #endif
  analogReference(EXTERNAL);
  pinMode(PIN_KEYPAD_MOSI_CS, OUTPUT);
  pinMode(PIN_KEYPAD_MISO_CS, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(PIN_NOKIA_DC, OUTPUT);
  pinMode(PIN_NOKIA_CE, OUTPUT);
  pinMode(PIN_NOKIA_RST, OUTPUT);
  pinMode(PIN_NOKIA_BL, OUTPUT);
  pinMode(SDA, OUTPUT); //A4
  pinMode(SCL, OUTPUT); //A5

  if (!RTC.isRunning()) {
    tmElements_t tm;
    tm.Day = BUILD_DAY;
    tm.Month = BUILD_MONTH;
    tm.Year = BUILD_YEAR;
    tm.Hour = BUILD_HOUR;
    tm.Minute = BUILD_MIN;
    tm.Second = BUILD_SEC;

    RTC.write(tm);
  }

  #if defined(SET_GFX_ASSETS)
    if (
      writeBitmap(LcdAssets::roborallyMainScreenAddress, LcdAssets::roborallyMainScreenLength, roborallyMainScreen)
      //&& writeBitmap(...)
    ) {
      //Nothing here
    }
    return;
  #endif

  MainApp mainApp;
  mainApp.execute();
}

void loop()
{
  //nothing in the loop
}

#if defined(SET_GFX_ASSETS)
bool writeBitmap(unsigned int address, unsigned short int length, const unsigned char* bitmap)
{
    // Write main screen
    for (unsigned short int i = 0; i < length; i++) {
        eeprom24c08.write_1_byte(address + i, bitmap[i]);
        lcd.clear(false);
        lcd.setCursor(0, 0);
        lcd.print("Writing GFX..");
        lcd.setCursor(0, 1);
        lcd.print("address");
        lcd.setCursor(0, 2);
        lcd.print((int)(address + i));
        delay(10);
    }
    lcd.clear(false);
    lcd.setCursor(0, 0);
    lcd.print("verify...");
    delay(500);

    for (unsigned short int i = 0; i < length; i++) {
        lcd.clear(false);
        lcd.setCursor(0, 0);
        lcd.print("Verify...");
        lcd.setCursor(0, 1);
        lcd.print("address");
        lcd.setCursor(0, 2);
        lcd.print((int)(address + i));
        byte writtenData = eeprom24c08.read_1_byte(address + i);
        if (writtenData != bitmap[i]) {
            lcd.clear(false);
            lcd.setCursor(0, 0);
            lcd.print("Wrong data");
            lcd.setCursor(0, 1);
            lcd.print(writtenData);
            lcd.setCursor(0, 2);
            lcd.print("expected ");
            lcd.print(bitmap[i]);
            lcd.setCursor(0, 3);
            lcd.print("at address");
            lcd.setCursor(0, 4);
            lcd.print(address + i);

            return false;
        }
        delay(10);
    }

    lcd.clear(false);
    lcd.print("Done");

    return true;
}
#endif