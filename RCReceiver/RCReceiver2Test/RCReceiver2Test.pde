#include <RCReceiver2.h>

void setup(){
  Serial.begin(9600);
  RCReceiverBegin(2);
  Serial.println("Beginning");
}

void loop(){
  if (changed == 1){
    Serial.println(risingFalling);
    changed = 0;
  }
}

