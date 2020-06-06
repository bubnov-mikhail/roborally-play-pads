#include <main.h>

//#define DEBUG true
//#define SET_CONFIG_DEFAULTS true
//#define SET_CLOCK true

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
ServiceContainer serviceContainer(&configStorage, &lcd, &keypad, &headline, PIN_BUZZER);
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
  pinMode(PIN_A4, OUTPUT);
  pinMode(PIN_A5, OUTPUT);

  #if defined(SET_CLOCK)
    tmElements_t tm;
    const char *monthName[12] = {
      "Jan", "Feb", "Mar", "Apr", "May", "Jun",
      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    int Hour, Min, Sec;

    if (sscanf(__TIME__, "%d:%d:%d", &Hour, &Min, &Sec) == 3) {
      tm.Hour = Hour;
      tm.Minute = Min;
      tm.Second = Sec;
    }

    char Month[12];
    int Day, Year;
    uint8_t monthIndex;

    if (sscanf(__DATE__, "%s %d %d", Month, &Day, &Year) == 3) {
      for (monthIndex = 0; monthIndex < 12; monthIndex++) {
        if (strcmp(Month, monthName[monthIndex]) == 0) break;
      }
      if (monthIndex < 12) {
        tm.Day = Day;
        tm.Month = monthIndex + 1;
        tm.Year = CalendarYrToTm(Year);
      }
    }
    RTC.write(tm);
  #endif
  
  MainApp mainApp;
  mainApp.execute();
}

void loop()
{
  //nothing in the loop
}