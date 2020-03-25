#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void initLcd() {
  lcd.begin(16, 2);             // initialize LCD
};

void LcdStart(float smoothTemperature) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("temp: ");
  double displayTemperature = truncf((smoothTemperature) * 10) / 10;
  lcd.print(displayTemperature, 1);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("heat: ");
  lcd.print((int)heatCycle);
  lcd.print(" ms");

  if (tempSelectorState == HIGH) {
    lcd.print("  ");
    lcd.print("S");
  }
};

byte arrow[] = {
  B00000,
  B00000,
  B01000,
  B01100,
  B01110,
  B01100,
  B01000,
  B00000
};

void LcdMenu( String str1, String str2) {
  lcd.clear();
  lcd.createChar(0, arrow);
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.print(str1);

  lcd.setCursor(1, 1);
  lcd.print(str2);
};

void LcdMenu( char str1[], char str2[]) {
  lcd.clear();
  lcd.createChar(0, arrow);
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.write(str1);

  lcd.setCursor(1, 1);
  lcd.write(str2);
};
