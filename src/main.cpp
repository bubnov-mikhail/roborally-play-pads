#include <main.h>

//#define SET_CONFIG_DEFAULTS true
#define DEFAULT_CONTRAST 50
//#define PAD_NUMBER 1
//#define SET_GFX_ASSETS true
//#define SET_GFX_ASSETS_PART_1 true
//#define SET_GFX_ASSETS_PART_2 true
#define Eeprom24C32_capacity 32 // Size in bytes
#define Eeprom24C32_address 0x50
#define Eeprom24C08_capacity 8 // Size in bytes
#define Eeprom24C08_address 0x54
//#define DEBUG true
#define DEFAULT_RADIO_CHANNEL 5
#define DEFAULT_RADIO_LEVEL 1
#define RADIO_ADDRESS 0
#if defined(SET_GFX_ASSETS)
#include "GfxAssets.h"
#include <ProgressBar.h>
#endif

/**
 * Init services
 */
#if defined(SET_CONFIG_DEFAULTS)
ConfigStorage configStorage(true, true, DEFAULT_CONTRAST, DEFAULT_RADIO_CHANNEL, DEFAULT_RADIO_LEVEL);
#else
ConfigStorage configStorage;
#endif

TonePlayer tonePlayer(&configStorage, PIN_BUZZER);
Nokia_LCD lcd(PIN_NOKIA_DC, PIN_NOKIA_CE, PIN_NOKIA_RST, PIN_NOKIA_BL);
Keypad keypad(&tonePlayer, PIN_KEYPAD_MOSI_CS, PIN_KEYPAD_MISO_DATA, PIN_KEYPAD_MOSI_SCK, PIN_BUZZER, configStorage.isWithSounds());
Headline headline(&configStorage, &lcd, &RTC, PIN_VOLTAGE_READ);
Eeprom24C eeprom24c32(Eeprom24C32_capacity, Eeprom24C32_address);
Eeprom24C eeprom24c08(Eeprom24C08_capacity, Eeprom24C08_address);
ByteLoader byteLoader32(&eeprom24c32);
ByteLoader byteLoader08(&eeprom24c08);
RF24 radio(PIN_RADIO_CE, PIN_RADIO_CS);
MenuRenderer menuRenderer(&lcd);
ServiceContainer serviceContainer(&configStorage, &lcd, &keypad, &headline, &byteLoader32, &byteLoader08, &RTC, &tonePlayer, &radio, &menuRenderer);
ServiceContainer *AbstractApp::sc = &serviceContainer;
uint8_t RoborallyApp::spiMoSiCs = PIN_KEYPAD_MOSI_CS;

char MainApp::backlightMenuName[14];
char MainApp::soundsMenuName[11];
char MainApp::radioChannelMenuName[13];
char MainApp::radioLevelMenuName[11];

AbstractApp::APPS nextApp = AbstractApp::APPS::MAIN_MENU;
AbstractApp::APPS MainApp::nextApp;

void setup()
{
#if defined(DEBUG)
  Serial.begin(9600);
#endif
  analogReference(EXTERNAL);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(SDA, OUTPUT); //A4
  pinMode(SCL, OUTPUT); //A5

  lcd.setBacklight(configStorage.isWithBacklight());
  lcd.begin();
  lcd.setContrast(configStorage.getContrast());

  keypad.begin();

  // Setup the radio module
  if (radio.begin())
  {
    configStorage.setRadioConnected(true);
    radio.setPALevel(configStorage.getRadioLevel());
    radio.setChannel(configStorage.getRadioChannel());
    radio.enableDynamicPayloads();
    radio.enableAckPayload();
    radio.powerDown();
  }
  else
  {
    configStorage.setRadioConnected(false);
  }

  if (!RTC.isRunning())
  {
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

  while (!keypad.read())
  {
    continue;
  }

  unsigned int total32Capacity = Eeprom24C32_capacity * 128;
  unsigned int total08Capacity = Eeprom24C32_capacity * 128;

  if (
#if defined(SET_GFX_ASSETS_PART_1)
      writeBytes(&eeprom24c32, LcdAssets::roborallyMainScreenAddress, total32Capacity, LcdAssets::roborallyMainScreenLength, roborallyMainScreenBitmap, false) && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveForwardAddress, total32Capacity, LcdAssets::roborallyMovesBitmapLength, roborallyMoveForwardBitmap, true) && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveForward2Address, total32Capacity, LcdAssets::roborallyMovesBitmapLength, roborallyMoveForward2Bitmap, true) && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveForward3Address, total32Capacity, LcdAssets::roborallyMovesBitmapLength, roborallyMoveForward3Bitmap, true) && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveBackAddress, total32Capacity, LcdAssets::roborallyMovesBitmapLength, roborallyMoveBackBitmap, true) && writeBytes(&eeprom24c32, LcdAssets::roborallyMovePowerDownAddress, total32Capacity, LcdAssets::roborallyMovesBitmapLength, roborallyMovePowerDownBitmap, true) && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveLeftAddress, total32Capacity, LcdAssets::roborallyMovesBitmapLength, roborallyMoveLeftBitmap, true) && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveRightAddress, total32Capacity, LcdAssets::roborallyMovesBitmapLength, roborallyMoveRightBitmap, true) && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveUTurnAddress, total32Capacity, LcdAssets::roborallyMovesBitmapLength, roborallyMoveUTurnBitmap, true) && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveWait1Address, total32Capacity, LcdAssets::roborallyMovesBitmapLength, roborallyMoveWait1Bitmap, true) && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveWait2Address, total32Capacity, LcdAssets::roborallyMovesBitmapLength, roborallyMoveWait2Bitmap, true) && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveWait3Address, total32Capacity, LcdAssets::roborallyMovesBitmapLength, roborallyMoveWait3Bitmap, true) && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveWait4Address, total32Capacity, LcdAssets::roborallyMovesBitmapLength, roborallyMoveWait4Bitmap, true) && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveWait5Address, total32Capacity, LcdAssets::roborallyMovesBitmapLength, roborallyMoveWait5Bitmap, true) && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveWait6Address, total32Capacity, LcdAssets::roborallyMovesBitmapLength, roborallyMoveWait6Bitmap, true) && writeBytes(&eeprom24c32, LcdAssets::roborallyMoveWait7Address, total32Capacity, LcdAssets::roborallyMovesBitmapLength, roborallyMoveWait7Bitmap, true) && writeBytes(&eeprom24c08, roborallyCardsAddress, total08Capacity, roborallyCardsLength, roborallyCards, false)
#else if defined(SET_GFX_ASSETS_PART_2)
      writeBytes(&eeprom24c32, LcdAssets::splashScreenAddress, total32Capacity, LcdAssets::fullScreenLength, splashScreenBitmap, false)
#endif
  )
  {
    //Nothing here
  }
  return;
#endif
}

void loop()
{
#if !defined(SET_GFX_ASSETS)
  AbstractApp *app;
  switch (nextApp)
  {
  case AbstractApp::APPS::CONTRAST:
    app = new ContrastCtlApp();
    break;
  case AbstractApp::APPS::CLOCK:
    app = new ClockSetupApp();
    break;
  case AbstractApp::APPS::RADIO_CHANNEL:
    app = new RadioChannelApp();
    break;
  case AbstractApp::APPS::RADIO_CHANNEL_SCAN:
    app = new RadioChannelScanApp();
    break;
  case AbstractApp::APPS::ROBORALLY:
    drawFullScreen(LcdAssets::fullScreenLength, LcdAssets::splashScreenAddress);
    tonePlayer.playTones(AudioAssets::splashScreenIntro, AudioAssets::splashScreenIntroLength, false);
    while (!keypad.read() || keypad.getKeypadCode() == 0)
    {
      // Animation on the splash screen...
    }
#if defined(PAD_NUMBER) && defined(PAD_ANOUNCE_SELF_PERIOD)
    app = new RoborallyApp(PAD_NUMBER, PAD_ANOUNCE_SELF_PERIOD);
#else
    app = new RoborallyApp(0, 1000);
#endif
    break;
  default:
    app = new MainApp();
    nextApp = app->execute();
    delete app;
    return;
  }

  app->execute();
  delete app;
  resetFunc();
#endif
}

void drawFullScreen(const unsigned short int length, unsigned int address)
{
  lcd.clear(false);
  headline.update(true);
  AbstractApp::sc->getMenuRenderer()->render_header(StringAssets::loading);
  ProgressBar *progressBar = new ProgressBar(&lcd, 10, 74, 3, true);
  progressBar->render(0);

  unsigned char *bitmap = new unsigned char[length];
  for (unsigned int i = 0; i < length; i++)
  {
    byteLoader32.loadByteToPosition(bitmap, address + i, i);
    progressBar->render(i * 100 / length);
    headline.update();
  }

  lcd.setCursor(0, 0);
  lcd.draw(bitmap, length, false);
  delete bitmap, progressBar;
}

#if defined(SET_GFX_ASSETS)
bool writeBytes(Eeprom24C *eeprom24C, unsigned int address, unsigned int totalCapacity, unsigned short int length, const unsigned char *bytes, bool doubleRows)
{
  ProgressBar *progressBarBitmap = new ProgressBar(&lcd, 10, 74, 1, true);
  ProgressBar *progressBarTotal = new ProgressBar(&lcd, 10, 74, 4, true);

  if (address + length > totalCapacity)
  {
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
  for (unsigned short int i = 0; i < length; i++)
  {
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

  for (unsigned short int i = 0; i < length; i++)
  {
    delay(5);
    byte writtenData = eeprom24C->read_1_byte(address + i);
    unsigned char *bitmap = new unsigned char[1];
    bitmap[0] = writtenData;
    if (doubleRows && i == length / 2)
    {
      lcd.setCursor(0, 1);
    }
    lcd.draw(bitmap, 1, false);
    delete bitmap;

    if (writtenData != bytes[i])
    {
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