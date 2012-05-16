#include <ArduBot.h>

void setup() {

}

void loop() {
  moveRobot(1,0,0,0);
  delay (5000);
  moveRobot(0,1,0,0);
  delay (5000);
}
