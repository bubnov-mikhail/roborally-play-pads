#include <main.h>

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
Eeprom24C eeprom24c32(Eeprom24C32_capacity, Eeprom24C32_address);
Eeprom24C eeprom24c08(Eeprom24C08_capacity, Eeprom24C08_address);
ByteLoader byteLoader32(&eeprom24c32);
ByteLoader byteLoader08(&eeprom24c08);
ServiceContainer serviceContainer(&configStorage, &lcd, &keypad, &headline, &byteLoader32, &byteLoader08, &RTC, PIN_BUZZER);
ServiceContainer *AbstractApp::sc = &serviceContainer;
RoborallyApp::GameStates RoborallyApp::gameState = RoborallyApp::CONNECTING;
uint8_t RoborallyApp::round = 0;

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
  Wire.setClock(400000);

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
      writeBytes(&eeprom24c32, LcdAssets::roborallyMainScreenAddress, total32Capacity, LcdAssets::roborallyMainScreenLength, roborallyMainScreen)
      && writeBytes(&eeprom24c08, roborallyCardsAddress, total08Capacity, roborallyCardsLength, roborallyCards)
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
bool writeBytes(Eeprom24C* eeprom24C, unsigned int address, unsigned int totalCapacity, unsigned short int length, const unsigned char* bytes) {
    ProgressBar* progressBarBitmap = new ProgressBar(&lcd, 10, 74, 1, true);
    ProgressBar* progressBarTotal = new ProgressBar(&lcd, 10, 74, 4, true);

    lcd.clear(false);
    lcd.setCursor(0, 0);
    lcd.print("Writing bytes");
    lcd.setCursor(0, 3);
    lcd.print("Total capacity");

    progressBarBitmap->render(0);
    progressBarTotal->render(0);

    for (unsigned short int i = 0; i < length; i++) {
        eeprom24C->write_1_byte(address + i, bytes[i]);
        progressBarBitmap->render(i * 100 / length);
        progressBarTotal->render(i * 100 / totalCapacity);
        delay(10);
    }
    lcd.clear(false);
    lcd.setCursor(0, 0);
    lcd.print("Verify...");
    progressBarBitmap->reset();
    delay(200);

    for (unsigned short int i = 0; i < length; i++) {
        progressBarBitmap->render(i * 100 / length);
        byte writtenData = eeprom24C->read_1_byte(address + i);
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
        delay(10);
    }

    lcd.clear(false);
    lcd.print("Done");

    delete progressBarBitmap;
    delete progressBarTotal;

    return true;
}
#endif