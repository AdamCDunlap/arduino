//#define debugMode //uncomment to send back some more data to computer
//#define labview
#define filter

#include <RunningPercentile.h> //used to elimiate bad signals from radio
//#include "ArduBot.h" //used to drive motors in correct directions
#include <AFMotor.h> //should already be included from prevoius
//#include "Read_Servo_Signals.h" //outputs the values of the RC Receiver
#include <Wire.h>
#include "declarations.h"


const int SerialRate = 50; //milliseconds between Serial sendbacks

const int fiveV = A3;
const int GND = A2; //power pins on compass

RunningPercentile LX_Med(13, 20);
RunningPercentile LY_Med(13, 20);
RunningPercentile RX_Med(13, 20);

const byte compassAddress = 0x21; //shifted right 1 from address in datasheet

int readCompass(){
  /*Returns the compass reading, 0 <= heading < 3600*/
  Wire.requestFrom(compassAddress, (byte)2);//request heading
  int heading = Wire.receive(); //read data one byte at a time and combine
  heading <<= 8;
  heading |= Wire.receive();
  return heading;
}

int processCompass(int absolute, boolean reset=false){
  /*Returns the relative angle since the function was called with reset = true
    0 <= angle < 3600
   *Must call once with reset=true before using*/
  static int zeroPoint; //keep zero point from loop to loop
  if(reset)
    zeroPoint = absolute; //put the zero point back to current position if we reset
  int relative = absolute - zeroPoint;
  if (relative < 0) //zeroPoint is always positive, so relative could
    relative += 3600; //not be >=3600 and we only have to add to normalize
  return relative;
}

void setup(){
  Serial.begin(115200);
    
  readReceiverBegin(13, 2, 3);
  //see Read_Servo_Signals.h
  
  pinMode(fiveV, OUTPUT); //compass pins
  pinMode(GND, OUTPUT);
  digitalWrite(fiveV, HIGH); //DO NOT CHANGE - WILL DESTROY COMPASS
  digitalWrite(GND, LOW); //DO NOT CHANGE - WILL DESTROY COMPASS
  Wire.begin(); //i2c for compass
  
  delay(1000); //give the compass and receiver time to start up, and
               //let the LV program get ready to detect if it's in debug mode
  
  processCompass(readCompass(), true); //set the initial zero point for compass reading

  #ifdef labview    //the labview program expects a 1 or a 0 to determine if the 
  #ifdef debugMode  //program is in debug mode so it can receive the extra data.
  Serial.print("1");
  #else //debugMode
  Serial.print("0");
  #endif //debugMode
  #endif //labview
}

void displayGraph(unsigned int value, char terminatingChar, int resolution = 3){
  /*Prints value, then a graph relative to the value to the Serial Monitor*/
  Serial.print(value);
  Serial.print("\t");
  for (int i = value; i > 0; i-=resolution){
    Serial.print(".");
  }
  Serial.println(terminatingChar);
}

void loop(){ 
  static unsigned long previousSerialTime; //time that Serial data was last sent back (ms)
  static unsigned long previousReadTime; //the time of the last good reading from the receiver (uS)
  static unsigned long currentTime_us = 0; //time the current loop started at (uS)
  unsigned long previousTime_us = currentTime_us; //time of the start of the previous loop (uS)
  currentTime_us = micros();               //calling the micros() or millis() functions disables interrupts
  unsigned long currentTime_ms = millis(); //for a short time, so its best to keep calls at a minimum
  unsigned int looptime = currentTime_us - previousTime_us; //time last loop took (uS)
  
  static int strafe = 0;
  static int forward = 0;
  static int rotate = 0;
  
  boolean compassReset = false; //don't reset unless we say so later

  unsigned int pulseLeftX = joyTimeLX();
  unsigned int pulseLeftY = joyTimeLY();
  unsigned int pulseRightX = joyTimeRX();
    
  #ifdef filter
  LX_Med.add(pulseLeftX);
  LY_Med.add(pulseLeftY);
  RX_Med.add(pulseRightX);

  int pulseLeftX_filt = LX_Med.getPercentile();
  int pulseLeftY_filt = LY_Med.getPercentile();
  int pulseRightX_filt = RX_Med.getPercentile();
  
  #else //filter
  int pulseLeftX_filt = pulseLeftX;
  int pulseLeftY_filt = pulseLeftY;
  int pulseRightX_filt = pulseRightX;
  #endif //filter
    
  if (1800 > pulseLeftX_filt  && pulseLeftX_filt  > 800 && //if there's a bad value, stop everything
      1700 > pulseLeftY_filt  && pulseLeftY_filt  > 900 &&
      1800 > pulseRightX_filt && pulseRightX_filt > 800){
      if((!(1350 > pulseLeftX_filt  && pulseLeftX_filt  > 1250) ||    //if the joystick is almost exactly in the center...
          !(1350 > pulseLeftY_filt  && pulseLeftY_filt  > 1250) ||
          !(1375 > pulseRightX_filt && pulseRightX_filt > 1275 )  )){ //do not move
        strafe = constrain(pulseLeftX_filt, 860, 1730);
        strafe = map(strafe, 860, 1740, -1000, 1000);
        forward = constrain(pulseLeftY_filt, 990, 1610);
        forward = map(forward, 990, 1610, 1000, -1000);
        rotate = constrain(pulseRightX_filt, 950, 1750);
        rotate = map(rotate, 950, 1750, 1000, -1000);
      }
      else{
        strafe = 0;
        forward = 0;
        rotate = 0;
      }
      previousReadTime = currentTime_us;
  }
  else if(currentTime_ms -  previousReadTime < 100000L) {} //If we get a bad value, assume it's just an error for 100ms and keep last values
  else{ //if this happens, the controller has been turned off. A watchdog of sorts.
    strafe = 0;
    forward = 0;
    rotate = 0;
//    compassReset = true; //if controller is cycled, reset the compass.
  }
  
  unsigned int absoluteAngle = readCompass();
  unsigned int relativeAngle = processCompass(absoluteAngle, compassReset);
  struct moveRobotOutputs moveRobotOutputs = moveRobot(-strafe, forward, rotate, relativeAngle); //move the robot
  
  if(currentTime_ms - previousSerialTime > SerialRate){
    #ifdef labview
    Serial.write((byte*)&relativeAngle, 2); //Sends values as binary data
    Serial.write((byte*)&absoluteAngle, 2);
    Serial.write((byte*)&strafe, 2);
    Serial.write((byte*)&forward, 2);
    Serial.write((byte*)&rotate, 2);
    Serial.write((byte*)&moveRobotOutputs.speeds.speed4, 2);
    Serial.write((byte*)&moveRobotOutputs.speeds.speed3, 2);
    Serial.write((byte*)&moveRobotOutputs.speeds.speed1, 2);
    Serial.write((byte*)&moveRobotOutputs.axis.x, 2);
    Serial.write((byte*)&moveRobotOutputs.axis.y, 2);
    Serial.write((byte*)&pulseLeftX_filt, 2);
    Serial.write((byte*)&pulseLeftY_filt, 2);
    Serial.write((byte*)&pulseRightX_filt, 2); 
    Serial.write((byte*)&pulseLeftX, 2);
    Serial.write((byte*)&pulseLeftY, 2);
    Serial.write((byte*)&pulseRightX, 2);
    Serial.write((byte*)&looptime, 2);
    #ifdef debugMode
      Serial.write((byte*)&case1, 2);
      Serial.write((byte*)&case2, 2);
      Serial.write((byte*)&case3, 2);
      Serial.write((byte*)&error, 2);
      Serial.write((byte*)&case4, 2);
    #endif //debugMode
    #else //labview
//    Serial.print(looptime);
//    Serial.print("\t");
//    Serial.print(strafe);
//    Serial.print("\t");
//    Serial.print(forward);
//    Serial.print("\t");
//    Serial.print(rotate);
//    Serial.println();
//    displayGraph(pulseRightX_filt, '|', 50);
//    Serial.print("Relative:\tAbsolute:\n");
    Serial.print(relativeAngle);
    Serial.print("\t\t");
    Serial.print(absoluteAngle);
    Serial.print("\t\t");
    Serial.print(forward);
    Serial.print("\t\t");
    Serial.print(strafe);
//    displayGraph(absoluteAngle, '|', 60);
    Serial.println();
    #endif //labview
    previousSerialTime = currentTime_ms;
  }
}

/*
* Debugging Info:
* If the signals are correct but on the wrong channels,
* make sure that the ORgate definition is correct.
*/
