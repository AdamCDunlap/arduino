#include <LEDArray.h>

LEDArray disp(9,8,10);

const unsigned long trials = 100000;

void setup(){
  Serial.begin(9600);
  unsigned long startTime;
  unsigned long elapsedTime;
  Serial.println("Starting");
  
  disp.setPrintType(FLASH);
  startTime = millis();
  for(unsigned long i=0;i<trials * 1;i++){
    disp.printRoutine();
  }
  elapsedTime = millis() - startTime;
  Serial.print("Flash. Total(ms) = ");Serial.print(elapsedTime); Serial.print(", time per character(ms) = "); Serial.println((float)elapsedTime/(float)trials);
  
  disp.setPrintType(FADE);
  startTime = millis();
  for(unsigned long i=0;i<trials * 30;i++){
    disp.printRoutine();
  }
  elapsedTime = millis() - startTime;
  Serial.print("Fade. Total(ms) = ");Serial.print(elapsedTime); Serial.print(", time per character(ms) = "); Serial.println((float)elapsedTime/(float)trials);
  
  disp.setPrintType(SCROLL);
  startTime = millis();
  for(unsigned long i=0;i<trials * 6;i++){
    disp.printRoutine();
  }
  elapsedTime = millis() - startTime;
  Serial.print("Scroll. Total(ms) = ");Serial.print(elapsedTime); Serial.print(", time per character(ms) = "); Serial.println((float)elapsedTime/(float)trials);
}

void loop(){}
