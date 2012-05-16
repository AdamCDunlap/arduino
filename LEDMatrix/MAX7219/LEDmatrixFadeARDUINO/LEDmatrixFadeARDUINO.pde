#include <MAX7219.h>
#include "Letters.h"

Matrix LEDdisplay(11, 10, 12, 1); // Set up 1 matrix with:
// Data  (DIN)  on pin 8
// Clock (CLK)  on pin 9
// Load  (LOAD) on pin 10

void setup(){
  //nothing
}

void loop(){
  LEDdisplay.drawPicPmem(letterA);
  for(int i=0;i<100;i++){
    LEDdisplay.setBrightness(i);
    delay(10);
  }
  for(int i=100;i>0;i--){
      LEDdisplay.setBrightness(i);
      delay(10);
  }
  LEDdisplay.drawPicPmem(letterR);
  for(int i=0;i<100;i++){
    LEDdisplay.setBrightness(i);
    delay(10);
  }
  for(int i=100;i>0;i--){
      LEDdisplay.setBrightness(i);
      delay(10);
  }
  LEDdisplay.drawPicPmem(letterD);
  for(int i=0;i<100;i++){
    LEDdisplay.setBrightness(i);
    delay(10);
  }
  for(int i=100;i>0;i--){
      LEDdisplay.setBrightness(i);
      delay(10);
  }
  LEDdisplay.drawPicPmem(letterU);
  for(int i=0;i<100;i++){
    LEDdisplay.setBrightness(i);
    delay(10);
  }
  for(int i=100;i>0;i--){
      LEDdisplay.setBrightness(i);
      delay(10);
  }
  LEDdisplay.drawPicPmem(letterI);
  for(int i=0;i<100;i++){
    LEDdisplay.setBrightness(i);
    delay(10);
  }
  for(int i=100;i>0;i--){
      LEDdisplay.setBrightness(i);
      delay(10);
  }
  LEDdisplay.drawPicPmem(letterN);
  for(int i=0;i<100;i++){
    LEDdisplay.setBrightness(i);
    delay(10);
  }
  for(int i=100;i>0;i--){
      LEDdisplay.setBrightness(i);
      delay(10);
  }
  LEDdisplay.drawPicPmem(letterO);
  for(int i=0;i<100;i++){
    LEDdisplay.setBrightness(i);
    delay(10);
  }
  LEDdisplay.clear();
  delay(1000);
}
