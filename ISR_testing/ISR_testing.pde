#include <avr/interrupt.h>

volatile unsigned long isrnum = 0;

SIGNAL(PCINT2_vect){
  Serial.print("ISR called! ");
  Serial.println(isrnum);
  isrnum++;  
}
void setup() {
  Serial.begin(9600);
  digitalWrite(4, HIGH); //Enable pullup on pin 4
  PCMSK0 = B00000000; //No pins will trigger PCINT0 interrupt
  PCMSK1 = B00000000; //No pins will trigger PCINT1 interrupt
  PCMSK2 = B00010000; //Digital pin 4 can trigger PCINT2 interrupt
  PCICR =  B00000100; //Enable interrupt for PCINT2
  Serial.println("Beginning Program!");
}

void loop() {
  static unsigned long loopnum = 0;
  delay(1000);
  Serial.print("Loop ");
  Serial.print(loopnum);
  Serial.println(" One second passed!");
  loopnum ++;
}
