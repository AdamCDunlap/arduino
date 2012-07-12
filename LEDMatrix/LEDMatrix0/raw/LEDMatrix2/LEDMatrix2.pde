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
    curMatrix[i] = pgm_read_byte_near(&blankScreen[i]);
  delay(offTime);
}

void setup(){
  for(int i = 0; i < numOfRows; i++){
    pinMode(rows[i], OUTPUT);
    pinMode(cols[i], OUTPUT);
  }
  Timer1.initialize(3000);
  Timer1.attachInterrupt(screenUpdate);
}

void loop(){
  displayChar(letterE, 500, 0);
  displayChar(letterL, 500, 0);
  displayChar(letterL, 500, 0);
  displayChar(letterE, 500, 0);
  displayChar(letterN, 500, 0);
  delay(300);
}

 void screenUpdate() {
   static byte currentCol = 0;
   for (int i = 0; i < numOfCols; i++)
     digitalWrite(cols[i], currentCol == i);
   for (int i = 0; i < numOfRows; i++)
     digitalWrite(rows[i], !bitRead(curMatrix[currentCol], i));
   currentCol = (currentCol + 1) % 8;
 }
