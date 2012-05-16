#include "RCReceiver.h"

void setup(){
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Serial.begin(115200);
  Serial.println("Starting");
  RCReceiverBegin(2, 3);
  Serial.println("Receiver Begun");
}

void loop(){
  for (int i=0;i<4;i++){
    Serial.print(pulseTime(i, 100000));
    Serial.print("\t");
  }
  Serial.println();
  delay(50);
}
