#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <words.h>
#include <main.h>

LiquidCrystal lcd(LCD_PIN_RS, LCD_PIN_E, LCD_PIN_DB4, LCD_PIN_DB5, LCD_PIN_DB6, LCD_PIN_DB7);

void setup()
{
    pinMode(BRT_INPUT, INPUT);
    pinMode(LCD_PIN_BRT, OUTPUT);
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print(lcd_txt_searching_players);
}

void loop()
{
  updateLcdBrightness();
}

// This function will update the LCD brightness
void updateLcdBrightness()
{
  int externalBright = max(300, analogRead(BRT_INPUT)); //R1(photo) + R2(100kOm)
  int lcdBright = map(externalBright, 100, 1024, 256, 0);
  analogWrite(LCD_PIN_BRT, lcdBright);
}
