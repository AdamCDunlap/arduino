#include "ArduBot.h"
#include <AFMotor.h>
/******************************************************************
****** moveRobot(x axis, y axis, rotation, gyroscope(radians))*****
******************************************************************/

void setup(){
}

void loop(){
  moveRobot(1,0,0,0);
  delay(1000);
  moveRobot(0,1,0,0);
  delay(1000);
  moveRobot(0,0,1,0);
  delay(1000);
}
