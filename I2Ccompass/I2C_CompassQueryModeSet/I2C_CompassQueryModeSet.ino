#include <Wire.h>
#define WRITE
#define READ

void setup(){
  Wire.begin();
  delay(1000);
  #ifdef WRITE
  Wire.beginTransmission(0x21); //communicate with compass
  Wire.write(0x77); //Write to EEPROM
//  byte errorOne = Wire.endTransmission();
//  delay(1000);
//  Wire.beginTransmission(0x21);
  Wire.write(0x08); //Write to Operation Mode Byte
//  Wire.endTransmission();
//  delay(1000);
//  Wire.beginTransmission(0x21);
  Wire.write(B00010000); //Standby Mode with Set/Reset
  byte errorTwo = Wire.endTransmission();
  delay(1000);
  #endif
  #ifdef READ
  Wire.beginTransmission(0x21);
  Wire.write(0x72); //Read from EEPROM
//  byte errorThree = Wire.endTransmission();
//  delay(1000);
//  Wire.beginTransmission(0x21);
  Wire.write(0x08); //Read Operation Mode byte
  byte errorFour = Wire.endTransmission();
  delay(1000);
  Wire.requestFrom(0x21, 1); //Request Operation mode byte
  delay(100);
  byte received = Wire.read(); //Get byte
  #endif
  Serial.begin(115200);
  Serial.println("--------------------------------");
//  Serial.println(errorOne, DEC);
  #ifdef WRITE
  Serial.println(errorTwo, DEC);
  #endif
//  Serial.println(errorThree, DEC);
  #ifdef READ
  Serial.println(errorFour, DEC);
  Serial.println(received, BIN);
  #endif
}
void loop(){}
