#include <main.h>

//#define DEBUG true

/**
 * Init services
 */
#if defined(SET_CONFIG_DEFAULTS)
  ConfigStorage configStorage(true, true, 50);
#else
  ConfigStorage configStorage;
#endif

TonePlayer tonePlayer(&configStorage, PIN_BUZZER);
Nokia_LCD lcd(SCK, MOSI, PIN_NOKIA_DC, PIN_NOKIA_CE, PIN_NOKIA_RST, PIN_NOKIA_BL);
Keypad keypad(&tonePlayer, PIN_KEYPAD_MOSI_CS, PIN_KEYPAD_MISO_CS, PIN_BUZZER, configStorage.isWithSounds());
Headline headline(&configStorage, &lcd, &RTC, PIN_VOLTAGE_READ);
Eeprom24C eeprom24c32(Eeprom24C32_capacity, Eeprom24C32_address);
Eeprom24C eeprom24c08(Eeprom24C08_capacity, Eeprom24C08_address);
ByteLoader byteLoader32(&eeprom24c32);
ByteLoader byteLoader08(&eeprom24c08);

ServiceContainer serviceContainer(&configStorage, &lcd, &keypad, &headline, &byteLoader32, &byteLoader08, &RTC, &tonePlayer);
ServiceContainer *AbstractApp::sc = &serviceContainer;
RoborallyApp::GameStates RoborallyApp::gameState = RoborallyApp::CONNECTING;
uint8_t RoborallyApp::spiMoSiCs = PIN_KEYPAD_MOSI_CS;

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
    tm.Year = BUILD_YEAR - ClockSetupApp::yearOffset;
    tm.Hour = BUILD_HOUR;
    tm.Minute = BUILD_MIN;
    tm.Second = BUILD_SEC;

    RTC.write(tm);
  }

  #if defined(SET_GFX_ASSETS)
    lcd.clear(false);
    lcd.setCursor(0, 0);
    lcd.print("Press any key");
    lcd.setCursor(0, 1);
    lcd.print("to write GFX");

    while(!keypad.read()) {
        continue;
    }

    unsigned int total32Capacity = Eeprom24C32_capacity * 128;
    unsigned int total08Capacity = Eeprom24C32_capacity * 128;
    if (
      writeBytes(&eeprom24c32, LcdAssets::roborallyMainScreenAddress, total32Capacity, LcdAssets::roborallyMainScreenLength, roborallyMainScreenBitmap, false)
      && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveForwardAddress, total32Capacity, LcdAssets::roborallyMovesLength, roborallyMoveForwardBitmap, true)
      && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveForward2Address, total32Capacity, LcdAssets::roborallyMovesLength, roborallyMoveForward2Bitmap, true)
      && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveForward3Address, total32Capacity, LcdAssets::roborallyMovesLength, roborallyMoveForward3Bitmap, true)
      && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveBackAddress, total32Capacity, LcdAssets::roborallyMovesLength, roborallyMoveBackBitmap, true)
      && writeBytes(&eeprom24c32, LcdAssets::roborallyMovePowerDownAddress, total32Capacity, LcdAssets::roborallyMovesLength, roborallyMovePowerDownBitmap, true)
      && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveLeftAddress, total32Capacity, LcdAssets::roborallyMovesLength, roborallyMoveLeftBitmap, true)
      && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveRightAddress, total32Capacity, LcdAssets::roborallyMovesLength, roborallyMoveRightBitmap, true)
      && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveUTurnAddress, total32Capacity, LcdAssets::roborallyMovesLength, roborallyMoveUTurnBitmap, true)
      && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveWait1Address, total32Capacity, LcdAssets::roborallyMovesLength, roborallyMoveWait1Bitmap, true)
      && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveWait2Address, total32Capacity, LcdAssets::roborallyMovesLength, roborallyMoveWait2Bitmap, true)
      && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveWait3Address, total32Capacity, LcdAssets::roborallyMovesLength, roborallyMoveWait3Bitmap, true)
      && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveWait4Address, total32Capacity, LcdAssets::roborallyMovesLength, roborallyMoveWait4Bitmap, true)
      && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveWait5Address, total32Capacity, LcdAssets::roborallyMovesLength, roborallyMoveWait5Bitmap, true)
      && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveWait6Address, total32Capacity, LcdAssets::roborallyMovesLength, roborallyMoveWait6Bitmap, true)
      && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveWait7Address, total32Capacity, LcdAssets::roborallyMovesLength, roborallyMoveWait7Bitmap, true)
      && writeBytes(&eeprom24c08, roborallyCardsAddress, total08Capacity, roborallyCardsLength, roborallyCards, false)
    ) {
      //Nothing here
    }
    return;
  #else
    MainApp mainApp;
    mainApp.execute();
  #endif
}

void loop()
{
  //nothing in the loop
}

#if defined(SET_GFX_ASSETS)
bool writeBytes(Eeprom24C* eeprom24C, unsigned int address, unsigned int totalCapacity, unsigned short int length, const unsigned char* bytes, bool doubleRows) {
    ProgressBar* progressBarBitmap = new ProgressBar(&lcd, 10, 74, 1, true);
    ProgressBar* progressBarTotal = new ProgressBar(&lcd, 10, 74, 4, true);

    if (address + length > totalCapacity) {
      lcd.clear(false);
      lcd.setCursor(0, 0);
      lcd.print("Wrong address");
      lcd.setCursor(0, 2);
      lcd.print(address);
      lcd.setCursor(0, 3);
      lcd.print("Total ");
      lcd.print(totalCapacity / 128);
      lcd.print(" kB only");

      return false;
    }
    lcd.clear(false);
    lcd.setCursor(0, 0);
    lcd.print("Writing bytes");
    lcd.setCursor(0, 3);
    lcd.print("Total ");
    lcd.print(totalCapacity / 128);
    lcd.print(" kB");

    progressBarBitmap->render(0);
    progressBarTotal->render(address * 100 / totalCapacity);

    delay(200);
    for (unsigned short int i = 0; i < length; i++) {
        eeprom24C->write_1_byte(address + i, bytes[i]);
        progressBarBitmap->render(i * 100 / length);
        progressBarTotal->render((address + i) * 100 / totalCapacity);
        delay(5);
    }
    lcd.clear(false);
    lcd.setCursor(0, 0);
    lcd.print("Verify...");
    lcd.setCursor(0, 0);
    delay(1000);
    lcd.clear(false);

    for (unsigned short int i = 0; i < length; i++) {
        delay(5);
        byte writtenData = eeprom24C->read_1_byte(address + i);
        unsigned char* bitmap = new unsigned char[1];
        bitmap[0] = writtenData;
        if (doubleRows && i == length / 2) {
          lcd.setCursor(0, 1);
        }
        lcd.draw(bitmap, 1, false);
        delete bitmap;

        if (writtenData != bytes[i]) {          
            lcd.clear(false);
            lcd.setCursor(0, 0);
            lcd.print("Wrong byte");
            lcd.setCursor(0, 1);
            lcd.print(writtenData);
            lcd.setCursor(0, 2);
            lcd.print("expected ");
            lcd.print(bytes[i]);
            lcd.setCursor(0, 3);
            lcd.print("at address");
            lcd.setCursor(0, 4);
            lcd.print(address + i);

            
            delete progressBarBitmap;
            delete progressBarTotal;

            return false;
        }
        
    }
    delay(3000);
    lcd.clear(false);
    lcd.print("Done");

    delete progressBarBitmap;
    delete progressBarTotal;

    return true;
}
#endif