#include <LEDArray.h>

LEDArray disp(9, 13, 10);

void setup(){
  disp.setPrintDuration(500);
  disp.setPrintType(SCROLL);
}

void loop(){
  static bool dispWasIn = true;
  if (!dispWasIn && digitalRead(2)) {
    disp.initialize();
  }
  if (digitalRead(2)) {
    disp.println("Adam is da bomb!!!");
  }
  dispWasIn = !digitalRead(2);
}
