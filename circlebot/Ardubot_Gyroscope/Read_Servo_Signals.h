#include <WProgram.h>

void ISRRUTT_end();

unsigned int servoRUTT_start;
unsigned int servoRUTT_val;
unsigned int servoTHRO_start;
unsigned int servoTHRO_val;

void ISRRUTT_begin() {
  servoRUTT_start = micros();
  detachInterrupt(0);
  attachInterrupt(0, ISRRUTT_end, FALLING); //start function ISRRUTT_end when pin 2 goes back down
}

void ISRRUTT_end() {
 servoRUTT_val = micros() - servoRUTT_start;
 detachInterrupt(0);
 attachInterrupt(0, ISRRUTT_begin, RISING);
}

unsigned int servoRUTT_val_f(void){
  return servoRUTT_val;
}

void ISRTHRO_end();

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

unsigned int servoTHRO_val_f(void){
  return servoTHRO_val;
}
