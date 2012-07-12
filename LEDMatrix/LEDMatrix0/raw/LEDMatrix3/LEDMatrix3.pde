#include <TimerOne.h>
#include "Letters.h"

const byte rows[] = {16, 17, 18, 19, 10, 11, 12, 13};
const byte cols[] = {9, 8, 7, 6, 5, 4, 3, 2};

const int numOfRows = sizeof(rows)/sizeof(rows[0]);
const int numOfCols = sizeof(cols)/sizeof(cols[0]);

byte curMatrix[8];

void displayChar(prog_uchar* letter, int onTime = 500, int offTime = 50){
  for(int i=0; i<8; i++)
    curMatrix[i] = pgm_read_byte_near(&letter[i]);
  delay(onTime);
  for(int i=0; i<8; i++)
    curMatrix[i] = 0;
  delay(offTime);
}

void setup(){
  Serial.begin(115200);
  for(int i = 0; i < numOfRows; i++){
    pinMode(rows[i], OUTPUT);
    pinMode(cols[i], OUTPUT);
  }
  Timer1.initialize(50000L);
  Timer1.attachInterrupt(screenUpdate);
}

void loop(){
  displayChar(letterS, 10000, 0);
//  displayChar(letterO);
//  displayChar(letterM);
//  displayChar(letterE);
//  displayChar(letterT);
//  displayChar(letterH);
//  displayChar(letterI);
//  displayChar(letterN);
//  displayChar(letterG);
//  delay(300);
//  displayChar(letterN);
//  displayChar(letterI);
//  displayChar(letterC);
//  displayChar(letterE);
//  displayChar(letterE);
//  displayChar(letterL);
//  displayChar(letterL);
//  displayChar(CHECKERBOARD1,170,0);
//  displayChar(CHECKERBOARD2,170,0);
//  delay(1000);
}

 void screenUpdate() {
   static volatile byte currentCol = 0;
   for (int i = 0; i < numOfCols; i++)
     digitalWrite(cols[i], currentCol == i);
   for (int i = 0; i < numOfRows; i++)
     digitalWrite(rows[i], !bitRead(curMatrix[currentCol], i));
   currentCol = (currentCol + 1) % 8;
 }
