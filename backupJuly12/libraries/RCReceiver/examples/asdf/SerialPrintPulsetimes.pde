#include <RCReceiver.h>

RCReceiver Futaba1C (2, 3);

void setup(){
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void loop(){
  for (int i=0;i<4;i++)
    Serial.print(Futaba1C.pulseTime(i));
  Serial.println();
}
