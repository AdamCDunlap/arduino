#include <LEDArray.h>

LEDArray disp(9, 8, 10, 1);

void setup(){
  disp.setPrintDuration(500);
  disp.setPrintType(SCROLL);
}

void loop(){
  disp.println("Adam is da bomb!!!");
}
