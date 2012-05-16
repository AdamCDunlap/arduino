#include <LEDArray.h>

LEDArray disp(9, 8, 10, 1);

void setup() 
{
  disp.setPrintDuration(900);
  disp.setPrintType(SCROLL);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()){
    char letter = Serial.read();
    Serial.print(letter);
    disp.print(letter, BYTE);
  }
}
