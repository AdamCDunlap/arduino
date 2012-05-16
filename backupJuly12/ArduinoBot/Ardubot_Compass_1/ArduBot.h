#include <AFMotor.h>
#include <math.h>
#include <WProgram.h>

AF_DCMotor motor1 (1, MOTOR12_1KHZ);
AF_DCMotor motor3 (3, MOTOR12_1KHZ);
AF_DCMotor motor4 (4, MOTOR12_1KHZ);

const long cosOneTwenty = -500;
const long sinOneTwenty = 866;
const long sinTwoForty = -866;
const long cosTwoForty = -500;

struct motorspeeds {int speed4; int speed3; int speed1;};
struct axis {int x; int y; int z;};
struct moveRobotOutputs{
  struct motorspeeds speeds;
  struct axis axis;
};

int runMtr(AF_DCMotor mtr, long spd) { //given a motor name and a speed -1,000,000 to 1,000,000, runs the motor at the speed
  uint8_t drctn;
  if      (spd < -1000000){spd = -1000000;}
  else if (spd >  1000000){spd =  1000000;}
  if (spd < 0) {drctn = BACKWARD;}
  else {drctn = FORWARD;}
  int fspd = fabs(spd) * .00018F + 75; //fspd is between 0 and 255
  if (fspd <= 75) {fspd = 0;}
  mtr.setSpeed(fspd); //set the speed
  mtr.run(drctn); //run the motor
  return fspd;
}

struct moveRobotOutputs moveRobot(int x, int y, int z, float angle=0) { //taking x, y, (-1000 to 1000),and an angle (in decaradians, 0 to 3600) values, runs the motors at speeds to move robot in correct direction
  struct motorspeeds speeds;
  struct axis axis;
  struct moveRobotOutputs Outputs;
  long zp = (long)z * 1000;
  long xp;
  long yp;
//  if (fabs(angle) > .1){ //only do the calculations if we supplied an angle
    angle = radians(angle/10);
    
    float cosangle = cos(angle);  
    float sinangle = sin (angle);
    xp = x * cosangle - y * sinangle; // rotating vectors
    yp = x * sinangle + y * cosangle; // with respect to angle  
//  }
//  else{
//    xp = x;
//    yp = y;
//  }
  long speed4 = sinTwoForty * yp + cosTwoForty * xp + zp;
  long speed3 = sinOneTwenty * yp + cosOneTwenty * xp + zp;
  long speed1 = xp * 1000 + zp; //normally, it would just be xp, but the trig values are all fractions of 1000
  
  axis.x = xp;
  axis.y = yp;
  axis.z = zp;
  
  long highSpeed = fabs(fmax(fmax(fabs(speed1),fabs(speed3)),fabs(speed4)));

  float speedDivisor = float(fmax(1000000, highSpeed))/1000000;
  
  speed4 /= speedDivisor;
//  speed4 *= 1000000;
  speed3 /= speedDivisor;
//  speed3 *= 1000000;
  speed1 /= speedDivisor;
//  speed1 *= 1000000;
  
  speeds.speed4 = runMtr(motor4, speed4);
  speeds.speed3 = runMtr(motor3, -speed3);
  speeds.speed1 = runMtr(motor1, speed1);
//  speeds.speed4 = speed4;
//  speeds.speed3 = speed3;
//  speeds.speed1 = speed1;
  Outputs.speeds = speeds;
  Outputs.axis = axis;
  return Outputs;
}
