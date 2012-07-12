//#include "Letters.h"

const byte dataPin = 8;
const byte clockPin = 9;
const byte LATCH = 10;

byte letterA[8] = {0x3C,0x7E,0xE7,0xC3,0xFF,0xFF,0xC3,0xC3};

void setup(){
  for(int i=8;i<14;i++){
    pinMode(i, OUTPUT);
  }
  digitalWrite(LATCH, LOW);
  delay(3000);
  shiftOut(dataPin, clockPin, MSBFIRST, 0x09); //Decode-Mode
  shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //all
  digitalWrite(LATCH, HIGH);
  digitalWrite(LATCH, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0x0A); //intensity
  shiftOut(dataPin, clockPin, MSBFIRST, 0x0F); //13/32
  digitalWrite(LATCH, HIGH);
  digitalWrite(LATCH, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0x0B); //Scan Limit
  shiftOut(dataPin, clockPin, MSBFIRST, 0x07); // 0-7
  digitalWrite(LATCH, HIGH);
  digitalWrite(LATCH, LOW);
//  shiftOut(dataPin, clockPin, MSBFIRST, 0x0C); //Shutdown
//  shiftOut(dataPin, clockPin, MSBFIRST, 0x01); //Off
//  digitalWrite(LATCH, HIGH);
//  digitalWrite(LATCH, LOW);
//  shiftOut(dataPin, clockPin, MSBFIRST, 0x0F); //Display Test
//  shiftOut(dataPin, clockPin, MSBFIRST, 0x01); //On (Should turn off shutdown)
//  digitalWrite(LATCH, HIGH);
//  digitalWrite(LATCH, LOW);
//  shiftOut(dataPin, clockPin, MSBFIRST, 0x0F); //Display Test
//  shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //Off
//  digitalWrite(LATCH, HIGH);
//  digitalWrite(LATCH, LOW);
  for(byte i=0;i<dataPin;i++){
    shiftOut(dataPin, clockPin, MSBFIRST, i+1); //Digit 0-7
    shiftOut(dataPin, clockPin, MSBFIRST, letterA[i]); //rows 0-7 of CHECKERBOARD1
    digitalWrite(LATCH, HIGH);
    digitalWrite(LATCH, LOW);
  }
    shiftOut(dataPin, clockPin, MSBFIRST, 0x0F); //Display Test
    shiftOut(dataPin, clockPin, MSBFIRST, 0x01); //On (Should turn off shutdown)
    digitalWrite(LATCH, HIGH);
    digitalWrite(LATCH, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0x0F); //Display Test
    shiftOut(dataPin, clockPin, MSBFIRST, 0x00); //Off
    digitalWrite(LATCH, HIGH);
    digitalWrite(LATCH, LOW);
    digitalWrite(dataPin, LOW);
}

void loop(){
//  digitalWrite(13, HIGH);
//  delay(500);
//  digitalWrite(13, LOW);
//  delay(500);
}
  
  
