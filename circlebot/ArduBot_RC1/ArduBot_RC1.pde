
#include "ArduBot.h"
#include <AFMotor.h>

unsigned long servoRUTT_start;
volatile int servoRUTT_val; 
double strafe;

unsigned long servoTHRO_start;
volatile int servoTHRO_val; 
double forward;

//unsigned long servoAILE_start;
//volatile unsigned servoAILE_val; 
//int adj_vaAILE;

unsigned long previousTime;
unsigned long currentTime;

void setup(){
  Serial.begin(9600);
  
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  
  attachInterrupt(0, ISRRUTT_begin, RISING);
  attachInterrupt(1, ISRTHRO_begin, RISING);
//  attachInterrupt(A2, ISRAILE_begin, RISING);
}


void ISRRUTT_begin() {
  servoRUTT_start = micros();
  detachInterrupt(0);
  attachInterrupt(0, ISRRUTT_end, FALLING);
}

void ISRRUTT_end() {
 servoRUTT_val = micros() - servoRUTT_start;
 detachInterrupt(0);
 attachInterrupt(0, ISRRUTT_begin, RISING);
}


void ISRTHRO_begin() {
  servoTHRO_start = micros();
  detachInterrupt(1);  
  attachInterrupt(1, ISRTHRO_end, FALLING);
}

void ISRTHRO_end() {
 servoTHRO_val = micros() - servoTHRO_start;
 detachInterrupt(1);
 attachInterrupt(1, ISRTHRO_begin, RISING);  
}

//void ISR3_begin() {
//  servo3_start = micros();
//  detachInterrupt(1);  
//  attachInterrupt(1, ISR2_end, FALLING);
//}
//
//void ISR3_end() {
// servo3_val = micros() - servo3_start;
// detachInterrupt(1);
// attachInterrupt(1, ISR3_begin, RISING); 
//}

void loop(){ 
  if (1800 > servoRUTT_val && servoRUTT_val > 800 && 1700 > servoTHRO_val && servoTHRO_val > 900){
    strafe = constrain(servoRUTT_val, 832, 1760); //map function ignores values outside its input min and max, so if this wasn't there we'd get a bunch of 828's instead of the expected 0s
    strafe = map(strafe, 832, 1760, -10000, 10000);
    strafe /= -10000; //map function does not deal with fractions, and value is negative
  
    forward = constrain(servoTHRO_val, 968, 1640); //map function ignores values outside its input min and max, so if this wasn't there we'd get a bunch of 964's instead of the expected 0s
    forward = map(forward, 968, 1640, -10000, 10000);
    forward /= -10000; //map function does not deal with fractions, and value is negative
  }
  
  else{
    strafe = 0;
    forward = 0;
  }
  moveRobot(strafe, forward, 0, 0);
  
  currentTime = micros();
  if(currentTime - previousTime > 100000){
    Serial.print("ServoRUTT Val: ");
    Serial.print(servoRUTT_val);
    Serial.print("\t");
    Serial.print("ServoTHRO Val: ");
    Serial.print(servoTHRO_val);
    //Serial.print("\t");
    //Serial.print("Servo3 Val:");
    //Serial.print(servo3_val);
    Serial.print("\t");
    Serial.print("X Movement: ");
    Serial.print(strafe);
    Serial.print("\t");
    Serial.print("Y Movement: ");
    Serial.print(forward);
//    Serial.print("\t");
//    Serial.print("Z Movement: ");
//    Serial.print(servoAILE_val);
    Serial.println();
  
    previousTime = currentTime;
  }
}
