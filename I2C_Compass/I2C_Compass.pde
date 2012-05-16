#include <Wire.h>
const int address = 0x21;
const int bytes = 2;
//#define LABVIEW

void displayGraph(unsigned int value, char terminatingChar, int resolution = 3){
  Serial.print(value);
  Serial.print("\t");
  for (int i = value; i > 0; i-=resolution){
    Serial.print(".");
  }
  Serial.println(terminatingChar);
}

void setup(){
  DDRC = 255;
  digitalWrite(A2, LOW);
  digitalWrite(A3, HIGH);
  Wire.begin();
  Serial.begin(115200);
}

unsigned int reading1 = 0;
unsigned int reading2 = 0;
unsigned int reading = 0;

void loop(){
  Wire.beginTransmission(address);
  Wire.send('A'); //make reading
  Wire.endTransmission();
  Wire.requestFrom(address, bytes); //request reading
  reading = Wire.receive(); //get MSB
  reading <<= 8;
  reading |= Wire.receive(); //get LSB
  #ifdef LABVIEW
  Serial.write((uint8_t*)&reading, 2);
  #else
  displayGraph(reading, '&', 50);
  #endif 
  delay(50);
}
