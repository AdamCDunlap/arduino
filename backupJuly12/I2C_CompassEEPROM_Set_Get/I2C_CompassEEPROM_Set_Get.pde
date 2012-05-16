#include <Wire.h>
//#define WRITE
#define READ

const byte address = 0x21; //from datasheet, shifted one bit right

const byte EEPROMbyte = B01110010;//continuous mode, 20hz, set/reset on

void setup(){
  DDRC = 255; //Sets all analog pins as output (same as pinMode)
  digitalWrite(A2, LOW); //Provides power
  digitalWrite(A3, HIGH);//to compass
  Wire.begin();
  delay(1000);
/**********************************************************/
  #ifdef WRITE
  Wire.beginTransmission(address); //communicate with compass
  Wire.send(0x77);       //Write to EEPROM
  Wire.send(0x08);       //Write to Operation Mode Byte
  Wire.send(EEPROMbyte); //Write byte itself
  byte errorWrite = Wire.endTransmission();
  delay(1000);
  #endif
/***********************************************************/
  #ifdef READ
  Wire.beginTransmission(address);
  Wire.send(0x72); //Read from EEPROM
  Wire.send(0x08); //Read Operation Mode byte
  byte errorRead = Wire.endTransmission();
  delay(1);
  Wire.requestFrom(address, (byte)1); //Request Operation mode byte
  byte received = Wire.receive(); //Get byte
  #endif
/***********************************************************/
  Serial.begin(115200);
  Serial.println("--------------------------------");
  #ifdef WRITE
  Serial.print("Write Error:\t");
  Serial.println(errorWrite, DEC);
  #endif
  #ifdef READ
  Serial.print("Read Error:\t");
  Serial.println(errorRead, DEC);
  Serial.print("Wrote byte:\t");
  Serial.println(EEPROMbyte, BIN);
  Serial.print("Received byte:\t");
  Serial.println(received, BIN);
  #endif
  Serial.end();
}
void loop(){}
