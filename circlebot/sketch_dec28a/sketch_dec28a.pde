#include "ArduBot.h"
#include <AFMotor.h>

void setup(){
}

void loop(){
  moveRobot(1,0,0,0);
  delay(3000);
  moveRobot(0,1,0,0);
  delay(3000);
}
