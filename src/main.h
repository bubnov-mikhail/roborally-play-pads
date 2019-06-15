#define LCD_PIN_RS 12 //Pin 4 On the LCD
#define LCD_PIN_E 11  //Pin 6 On the LCD
#define LCD_PIN_DB4 5 //Pin 11 On the LCD
#define LCD_PIN_DB5 4 //Pin 12 On the LCD
#define LCD_PIN_DB6 3 //Pin 13 On the LCD
#define LCD_PIN_DB7 2 //Pin 14 On the LCD

#define LCD_PIN_BRT 10 //Arduino pin to write the LCD brightness
#define BRT_INPUT A0   //Arduino pin to read external brightness

void updateLcdBrightness();
