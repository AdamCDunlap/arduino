#include <AFMotor.h>

int looptime;
long lastTime = 0;
long lastWriteTime = 0;
long lastReadTime = 0;

AF_DCMotor motor1 (1, MOTOR12_1KHZ);
AF_DCMotor motor3 (3, MOTOR12_1KHZ);
AF_DCMotor motor4 (4, MOTOR12_1KHZ);

byte speed1 = 0;
byte drctn1 = 0;
byte speed2 = 0;
byte drctn2 = 0;
byte speed3 = 0;
byte drctn3 = 0;
uint8_t drctn1flag;
uint8_t drctn2flag;
uint8_t drctn3flag;

uint8_t numToDrctn(byte x){
  x = x & 1;
  uint8_t drctn;
  if (x == 0){drctn = FORWARD;}
  else {drctn = BACKWARD;}
  return drctn;
}

void setup(){
  Serial.begin(115200);
}

void loop(){
  looptime = millis() - lastTime;
  
  int arrivedBytes = Serial.available();
  
  while (arrivedBytes >= 6){
    speed1 = Serial.read();
    drctn1flag = Serial.read();
    speed2 = Serial.read();
    drctn2flag = Serial.read();
    speed3 = Serial.read();
    drctn3flag = Serial.read();
    arrivedBytes = Serial.available();
  }
//  else if (arrivedBytes > 6){
//    Serial.flush();
//  }

  drctn1 = numToDrctn(drctn1flag);
  drctn2 = numToDrctn(drctn2flag);
  drctn3 = numToDrctn(drctn3flag);

  motor1.setSpeed(speed1);
  motor3.setSpeed(speed2);
  motor4.setSpeed(speed3);
  motor1.run(drctn1);
  motor3.run(drctn2);
  motor4.run(drctn3);
  
//  if (millis() - lastWriteTime >= 50){
//    Serial.write((unsigned char*)&speed1, 1);
//    Serial.write((unsigned char*)&speed2, 1);
//    Serial.write((unsigned char*)&speed3, 1);
//    lastWriteTime = millis();
//  }
  
  lastTime = millis();
}
