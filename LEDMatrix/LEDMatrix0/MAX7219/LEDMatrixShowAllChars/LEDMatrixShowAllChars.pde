#include <LEDArray.h>

LEDArray disp(9, 8, 10, 1);

void setup() 
{
  disp.setPrintDuration(900);
  disp.setPrintType(SCROLL);
}

void loop() {
  for(int i=0;i<256;i++){
    disp.print(i, BYTE);
  }
}
