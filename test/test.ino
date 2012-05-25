#include <AFMotor.h>

AF_DCMotor frontswivel(2);
AF_DCMotor rearswivel(4);
AF_DCMotor leftmove(3);
AF_DCMotor rightmove(1);

void setup() {
  digitalWrite(13, HIGH);
  while(!digitalRead(13));
  delay(100);
  while(digitalRead(13));
  frontswivel.setSpeed(255);
  frontswivel.run(FORWARD);
  while(!digitalRead(13)); // wait for press
  frontswivel.run(RELEASE);
  delay(100); // wait for debounce
  while(digitalRead(13)); // wait for release
  rearswivel.setSpeed(255);
  rearswivel.run(FORWARD);
  while(!digitalRead(13));
  rearswivel.run(RELEASE);
  delay(100);
  while(digitalRead(13));
}

void loop() {
  
}
