#include <RunningMedian.h>
#include "ArduBot.h"
#include <AFMotor.h>
#include "Read_Servo_Signals.h"

const int fiveV = A0;
const int GND = A1;
const int RATE = A2;
const int TEMP = A3;
const int STtwo = A4;
const int STone = A5; // Pins on Gyroscope

unsigned long previousTime; //Time since epoch of last loop, in milliseconds
unsigned long currentTime; //Time since epoch of current loop, in milliseconds
int looptime; //time last loop took, in milliseconds
unsigned long currentSerialTime;
unsigned long previousSerialTime;
unsigned long previousReadTime;

unsigned int servoRUTT_val1;
unsigned int servoRUTT_val2;
unsigned int servoRUTT_val3;

unsigned int servoTHRO_val1;
unsigned int servoTHRO_val2;
unsigned int servoTHRO_val3;

int maxRate = 3830;
int offset = 509;
//Needed for gyroscope

void setup(){
  Serial.begin(9600);
  
  pinMode(fiveV, OUTPUT);
  pinMode(GND, OUTPUT);
  pinMode(RATE, INPUT);
  pinMode(TEMP, INPUT);
  pinMode(STtwo, OUTPUT);
  pinMode(STone, OUTPUT);
  
  digitalWrite(fiveV, HIGH); //DO NOT CHANGE - WILL DESTROY GRYOSCOPE
  digitalWrite(GND, LOW); //DO NOT CHANGE - WILL DESTROY GYROSCOPE
  digitalWrite(STtwo, LOW);
  digitalWrite(STone, LOW);
  //startTime = millis();
  
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  
  attachInterrupt(0, ISRRUTT_begin, RISING); //start function ISRRUTT_begin when pin 2 goes from LOW to HIGH
  attachInterrupt(1, ISRTHRO_begin, RISING); //start function ISRTHRO_begin when pin 3 goes from LOW to HIGH
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

struct angleRate {long angle; long rate;};

struct angleRate readgyro(){
  static long angle; //millidegrees, between 0 and 360,000
  long rate; //millidegrees per second
  int rateReading; //0-1024, raw value from gyroscope
  struct angleRate AR;
  rateReading = analogRead(RATE);
  rate = (long (maxRate) * (rateReading - offset)) >> 9;
  angle += (rate * looptime)/1000;
  while (angle < 0){
    angle += 6283; //add 2pi until greater or equal to zero
  }
  angle %= 6283; //once >= zero, make sure it's not above 2pi
  AR.angle = angle;
  AR.rate = rate;
  return AR;
}

void loop(){ 
  currentTime = millis();
  looptime = currentTime - previousTime;
  
  int strafe = 0;
  int forward = 0;
  
  servoRUTT_val = servoRUTT_val_f();
  servoTHRO_val = servoTHRO_val_f();
    
  unsigned int servoRUTT_val_filt = max(max(max(/*max(max(servoRUTT_val5, servoRUTT_val4),servoRUTT_val3)*/servoRUTT_val3,servoRUTT_val2),servoRUTT_val1),servoRUTT_val);
  unsigned int servoTHRO_val_filt = max(max(max(/*max(max(servoTHRO_val5, servoTHRO_val4),servoTHRO_val3)*/servoTHRO_val3,servoTHRO_val2),servoTHRO_val1),servoTHRO_val);
  
  if (1800 > servoRUTT_val_filt && servoRUTT_val_filt > 800 && //if there's a bad value, stop everything
      1700 > servoTHRO_val_filt && servoTHRO_val_filt > 900){
      if(!(1350 > servoRUTT_val_filt&& servoRUTT_val_filt > 1250)){ //if the joystick is basically centered, say it's zero
        strafe = constrain(servoRUTT_val_filt, 860, 1730);
        strafe = map(strafe, 860, 1730, -1000, 1000);
        //strafe *= -1;
      }
      else{
        strafe = 0;
      }
      if(!(1350 > servoTHRO_val_filt && servoTHRO_val_filt > 1250)){ //if the joystick is basically centered, say it's zero
        forward = constrain(servoTHRO_val_filt, 990, 1610);
        forward = map(forward, 990, 1610, -1000, 1000);
        forward *= -1;
    }
      else{
        forward = 0;
      }
    previousReadTime = millis();
  }
  else if(millis() -  previousReadTime < 100) {} //If we get a bad value, assume it's just an error for 100ms and keep last values
  else{ //if this happens, the controller has been turned off. A watchdog of sorts.
    strafe = 0;
    forward = 0;
  }
//  strafe = 1000;
//  forward = 0;

  int bytesavailable = Serial.available();
  while (bytesavailable >= 4 ){
    maxRate = Serial.read() | (Serial.read() << 8);
    offset = Serial.read() | (Serial.read() << 8);
    bytesavailable = Serial.available();
  }
  Serial.flush();
  
  struct angleRate AR = readgyro();
  struct moveRobotOutputs moveRobotOutputs = moveRobot(strafe, forward, AR.angle);
  
  if(currentTime - previousSerialTime > 10){
    Serial.write((unsigned char*)&AR.angle, 4); //Sends angle as binary data
    Serial.write((unsigned char*)&AR.rate, 4); //Sends rate as binary data
    Serial.write((unsigned char*)&strafe, 2);
    Serial.write((unsigned char*)&forward, 2);
    Serial.write((unsigned char*)&moveRobotOutputs.speeds.speed4, 4);
    Serial.write((unsigned char*)&moveRobotOutputs.speeds.speed3, 4);
    Serial.write((unsigned char*)&moveRobotOutputs.speeds.speed1, 4);
    Serial.write((unsigned char*)&moveRobotOutputs.axis.x, 2);
    Serial.write((unsigned char*)&moveRobotOutputs.axis.y, 2);
    Serial.write((unsigned char*)&servoRUTT_val_filt, 2);
    Serial.write((unsigned char*)&servoTHRO_val_filt, 2);
//    Serial.print("ServoRUTT Val: ");
//    Serial.print(servoRUTT_val_filt);
//    Serial.print("\t");
//    Serial.print("ServoTHRO Val: ");
//    Serial.print(servoTHRO_val_filt);
//    //Serial.print("\t");
//    //Serial.print("Servo3 Val:");
//    //Serial.print(servo3_val);
//    Serial.print("\t");
//    Serial.print("X Movement: ");
//    Serial.print(strafe);
//    Serial.print("\t");
//    Serial.print("Y Movement: ");
//    Serial.print(forward);
////    Serial.print("\t");
////    Serial.print("Z Movement: ");
////    Serial.print(servoAILE_val);
//    Serial.print("\t");
//    Serial.print("Gyro Angle: ");
//    Serial.print(AR.angle);
//    Serial.print("\t");
//    Serial.print("Rate: ");
//    Serial.print(AR.rate);
//    Serial.print("\n");
  
    previousSerialTime = currentTime;
  }

  servoRUTT_val3 = servoRUTT_val2;
  servoRUTT_val2 = servoRUTT_val1;
  servoRUTT_val1 = servoRUTT_val;
  
  servoTHRO_val3 = servoTHRO_val2;
  servoTHRO_val2 = servoTHRO_val1;
  servoTHRO_val1 = servoTHRO_val;
  
  previousTime = currentTime;
}
