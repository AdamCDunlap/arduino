#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 10, 5, 4, 3, 2);

byte character[8] = 
{
  B00000,
  B00000,
  B00001,
  B00001,
  B00100,
  B00100,
  B10000,
  B10000,
};

//byte pixel[8] = {1,254,1,254,1,254,1,154};

void setup() {
  lcd.createChar(0, character);
  lcd.begin(16, 2);
  lcd.clear();
//  for(int i=0;i<16;i++)
//    lcd.write(0);
//  lcd.setCursor(0,1);
//  for (int i=0;i<16;i++)
//    lcd.write(0);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  lcd.print("HI,world.");
}

void loop() {
//  delay(1000);
//  lcd.clear();
//  lcd.setCursor(0,0);
//  lcd.print("llllll");
//  delay(1000);
//  lcd.clear();
//  lcd.print("HI, world.");
}

