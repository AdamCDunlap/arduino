#include <AFMotor.h>

AF_DCMotor motor1 (1, MOTOR12_1KHZ);
AF_DCMotor motor2 (2, MOTOR12_1KHZ);
AF_DCMotor motor3 (3, MOTOR12_1KHZ);

void setup() {
  
}
void loop() {
  motor1.setSpeed(255);
  motor2.setSpeed(255);
  motor3.setSpeed(255);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  delay(1000);
  
  motor3.run(RELEASE);
  motor2.run(BACKWARD);
  delay(2000);
  
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  delay(2000);
  
  motor2.run(RELEASE);
  motor3.run(FORWARD);
  delay(2000);
  
  motor1.run(FORWARD);
  motor3.run(BACKWARD);
  delay(2000);
  
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
}
