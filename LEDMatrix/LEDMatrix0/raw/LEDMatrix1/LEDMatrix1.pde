#include <TimerOne.h>

const int row1 = 16;
const int row2 = 17;
const int row3 = 18;
const int row4 = 19;
const int row5 = 10;
const int row6 = 11;
const int row7 = 12;
const int row8 = 13;
const int col1 = 2;
const int col2 = 3;
const int col3 = 4;
const int col4 = 5;
const int col5 = 6;
const int col6 = 7;
const int col7 = 8;
const int col8 = 9;

byte led[8] = {
  B11111111,
  B10000001,
  B10111101,
  B10100101,
  B10100101,
  B10111101,
  B10000001,
  B11111111
};

void setup(){
  for(int i=2;i<=19;i++)
    pinMode(i, OUTPUT);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(screenUpdate);
}

void loop(){
  for (int i=0;i<8;i++)
    led[i] = ~led[i];
  delay(500);
}

void screenUpdate(){
  volatile static byte row = 0;
  switch (row){
    case 0: digitalWrite(row1, LOW); break;
    case 1: digitalWrite(row2, LOW); break;
    case 2: digitalWrite(row3, LOW); break;
    case 3: digitalWrite(row4, LOW); break;
    case 4: digitalWrite(row5, LOW); break;
    case 5: digitalWrite(row6, LOW); break;
    case 6: digitalWrite(row7, LOW); break;
    case 7: digitalWrite(row8, LOW); break;
  }
  for (int i = 0; i < sizeof(led); i++){
    int currentPin;
    switch (i){
      case 0: currentPin = col1; break;
      case 1: currentPin = col2; break;
      case 2: currentPin = col3; break;
      case 3: currentPin = col4; break;
      case 4: currentPin = col5; break;
      case 5: currentPin = col6; break;
      case 6: currentPin = col7; break;
      case 7: currentPin = col8; break;
    }
    for (int k = 0; k < 0; k++){
      digitalWrite(currentPin, bitRead(led[i], k));
    }
  }
  ++row % 8;
}
