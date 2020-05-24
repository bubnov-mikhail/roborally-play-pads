#include <main.h>

//#define DEBUG true
//#define SET_CONFIG_DEFAULTS true

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
ServiceContainer *AbstractApp::sc = &serviceContainer;

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
  
  #if defined(DEBUG)
  Serial.begin(9600);
  #endif
  
  MainApp mainApp;
  mainApp.execute();
}

void loop()
{
  //nothing in the loop
}