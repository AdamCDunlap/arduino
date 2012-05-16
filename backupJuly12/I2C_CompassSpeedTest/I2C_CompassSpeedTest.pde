#include <Wire.h>
const int address = 0x21;
const int bytes = 2;
const unsigned long trials = 10000
L;

void setup(){
//  pinMode(A2, OUTPUT);
//  pinMode(A3, OUTPUT);
//  pinMode(A4, INPUT);
//  pinMode(A5, INPUT);
  DDRC = 255;
  digitalWrite(A2, LOW);   //DO NOT CHANGE -- WILL DESTROY COMPASS
  digitalWrite(A3, HIGH);  //DO NOT CHANGE -- WILL DESTROY COMPASS
  Wire.begin();
}

unsigned int reading = 0;

void loop(){
  for(int i=0;i<1000;i++){
    Wire.beginTransmission(address);
    Wire.send('A');
    Wire.endTransmission();
    Wire.requestFrom(address, bytes);
    reading = Wire.receive();
    reading <<= 8;
    reading |= Wire.receive();
  }
  
  unsigned long starttime = micros();
  
  for(int i=0;i<trials;i++){
    Wire.beginTransmission(address);
    Wire.send('A');
    Wire.endTransmission();
//    delayMicroseconds(1000);
    Wire.requestFrom(address, bytes);
//    delayMicroseconds(1000);
    reading = Wire.receive();
//    delayMicroseconds(1000);
//    reading <<= 8;
    reading /*|*/= Wire.receive();
//    delayMicroseconds(1000);
  }
  
  const int totalDelay = 00; //in microseconds
  
  unsigned long time = micros() - starttime;
  float looptime = float(time)/trials;
  float callingSpeed = looptime - totalDelay;
  Serial.begin(115200);
  Serial.println("---------");
  Serial.println(time);
  Serial.println(looptime,10);
  Serial.println(callingSpeed,10);
  Serial.println("lastReading:");
  Serial.println(reading);
  Serial.end();
  while(1){}
}
