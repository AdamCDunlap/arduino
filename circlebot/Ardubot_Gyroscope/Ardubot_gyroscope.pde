#include <RunningPercentile.h>
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

RunningPercentile RUTT_Med = RunningPercentile(4);
RunningPercentile THRO_Med = RunningPercentile(4);

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
  
  pinMode(13, OUTPUT); //for receiver
  
  digitalWrite(fiveV, HIGH); //DO NOT CHANGE - WILL DESTROY GRYOSCOPE
  digitalWrite(GND, LOW); //DO NOT CHANGE - WILL DESTROY GYROSCOPE
  digitalWrite(STtwo, LOW);
  digitalWrite(STone, LOW);
  digitalWrite(13, HIGH); //DO NOT CHANGE - WILL DESTROY RECEIVER
  
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  
  attachInterrupt(0, ISRRUTT_begin, RISING); //start function ISRRUTT_begin when pin 2 goes from LOW to HIGH
  attachInterrupt(1, ISRTHRO_begin, RISING); //start function ISRTHRO_begin when pin 3 goes from LOW to HIGH
  //see Read_Servo_Signals.h
}

struct angleRate {long angle; long rate;};

struct angleRate readgyro(boolean reset){
  static long angle; //millidegrees, between 0 and 360,000
  long rate; //millidegrees per second
  int rateReading; //0-1024, raw value from gyroscope
  struct angleRate AR;
  if (reset){angle = 0;}
  rateReading = analogRead(RATE);
  rate = (long (maxRate) * (rateReading - offset)) >> 9;
  angle += (rate * looptime)/1000;
  while (angle < 0){angle += 6283;} //add 2pi until greater or equal to zero
  angle %= 6283; //once >= zero, make sure it's not above 2pi
  AR.angle = angle;
  AR.rate = rate;
  return AR;
}

void loop(){ 
  currentTime = millis();
  looptime = currentTime - previousTime;
  
  static int strafe = 0;
  static int forward = 0;
  
  boolean gyroReset = false;
  
  servoRUTT_val = servoRUTT_val_f();
  servoTHRO_val = servoTHRO_val_f();
    
    RUTT_Med.add(servoRUTT_val);
    THRO_Med.add(servoTHRO_val);
    
    unsigned int servoRUTT_val_filt = RUTT_Med.getPercentile();
    unsigned int servoTHRO_val_filt = THRO_Med.getPercentile();
    
  if (1800 > servoRUTT_val_filt && servoRUTT_val_filt > 800 && //if there's a bad value, stop everything
      1700 > servoTHRO_val_filt && servoTHRO_val_filt > 900){
        if((!(1350 > servoRUTT_val_filt && servoRUTT_val_filt > 1250) || //if the joystick is almost exactly in the center,
        !(1350 > servoTHRO_val_filt && servoTHRO_val_filt > 1250))){ //do not move
          strafe = constrain(servoRUTT_val_filt, 860, 1730);
          strafe = map(strafe, 860, 1740, -1000, 1000);
          strafe *= -1;
          forward = constrain(servoTHRO_val_filt, 990, 1610);
          forward = map(forward, 990, 1610, -1000, 1000);
          forward *= -1;
        }
      else{
        strafe = 0;
        forward = 0;
      }
      previousReadTime = millis();
  }
  else if(millis() -  previousReadTime < 100) {} //If we get a bad value, assume it's just an error for 100ms and keep last values
  else{ //if this happens, the controller has been turned off. A watchdog of sorts.
    strafe = 0;
    forward = 0;
    gyroReset = true; //if controller is cycled, reset the gyroscope.
  }

  while (Serial.available() >= 4 ){
    maxRate = Serial.read() | (Serial.read() << 8);
    offset = Serial.read() | (Serial.read() << 8);
  }
  Serial.flush();
  
  struct angleRate AR = readgyro(gyroReset);
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
    Serial.write((unsigned char*)&servoRUTT_val,2);
    Serial.write((unsigned char*)&servoTHRO_val,2); 
    previousSerialTime = currentTime;
  }
  
  previousTime = currentTime;
}
