#include <AFRobot.h>

AF_DCMotor motor1 (1, MOTOR12_1KHZ);
AF_DCMotor motor2 (2, MOTOR12_1KHZ);
AF_DCMotor motor3 (3, MOTOR12_1KHZ);

float cosThirty = 1.732/2;
float sinThirty = .5;
float sinOneFifty = .5;
float cosOneFifty = -1.73/2;

void runMtr(AF_DCMotor mtr, float spd) { //given a motor name and a speed -1 to 1, runs the motor at the speed
  mtr.setSpeed(abs(spd*175)); //set the speed
  uint8_t drctn;
  if (spd < 0) {drctn = BACKWARD;}
  else {drctn = FORWARD;}
  mtr.run(drctn); //run the motor
}

void moveRobot(float x, float y, float rotation, float gyro) { //taking x, y, rotation (-1 to 1) , and gyro (in radians) values, runs the motors at speeds to move robot in correct direction with correct rotation
  //float xp = x * cos(gyro) - y * sin(gyro); // rotating vectors
  //float yp = x * sin(gyro) + y * cos(gyro); // with respect to gyro angle
  float xp = x;
  float yp = y;
  runMtr(motor1, sinThirty * yp - cosThirty * xp + rotation);
  runMtr(motor2, sinOneFifty* yp - cosOneFifty* xp + rotation);
  runMtr(motor3, xp + rotation);
}