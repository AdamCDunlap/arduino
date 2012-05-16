#include <WProgram.h>

/* Hardware Setup:
 * Each receiver output is attached to a digital pin
 * There is a diode between each receiver output and an external interrupt pin, with the anode on the arduino side
 * This creates a logical OR, so the processPulse function will run when any of the pulses happen
 * It will then decide whether it was going high or low, then which pin it was, and set the aproperate variables
 
 * Call "servo****_val_f()" to get pulse times in microseconds
 * Must call attachInterrupt(processPulse, *, CHANGE) where * is the interrupt pin (0 or 1)
 */

const int interrupt_pin = 2;
const int servoRUTT_pin = 3;
const int servoTHRO_pin = 9;
const int servoAILE_pin = 10;

unsigned int servoRUTT_start;
unsigned int servoRUTT_val;
unsigned int servoTHRO_start;
unsigned int servoTHRO_val;
unsigned int servoAILE_start;
unsigned int servoAILE_val;
byte currentPulse;

void processPulse(){ //activated on a changed pulse
  if (digitalRead(interrupt_pin)){ //was it rising edge?
    if (digitalRead(servoRUTT_pin)){
      currentPulse = 0; //tells which pulse happened for when it goes LOW
      servoRUTT_start = micros();
    }
    else if (digitalRead(servoTHRO_pin)){
      currentPulse = 1;
      servoTHRO_start = micros();
    }
    else if (digitalRead(servoAILE_pin)){
      currentPulse = 2;
      servoAILE_start = micros();
    }
  }
  else{ //pulse was falling edge
    if (currentPulse == 0){ //which pulse was that?
      servoRUTT_val = micros() - servoRUTT_start;
    }
    if (currentPulse == 1){
      servoTHRO_val = micros() - servoTHRO_start;
    }
    if (currentPulse == 2){
      servoAILE_val = micros() - servoAILE_start;
    }
  }
}

unsigned int servoRUTT_val_f(void){
  return servoRUTT_val;
}

unsigned int servoTHRO_val_f(void){
  return servoTHRO_val;
}

unsigned int servoAILE_val_f(void){
  return servoAILE_val;
}

//void ISRRUTT_end();
//
//void ISRRUTT_begin() {
//  servoRUTT_start = micros();
//  detachInterrupt(0);
//  attachInterrupt(0, ISRRUTT_end, FALLING); //start function ISRRUTT_end when pin 2 goes back down
//}
//
//void ISRRUTT_end() {
// servoRUTT_val = micros() - servoRUTT_start;
// detachInterrupt(0);
// attachInterrupt(0, ISRRUTT_begin, RISING);
//}

//void ISRTHRO_end();
//
//void ISRTHRO_begin() {
//  servoTHRO_start = micros();
//  detachInterrupt(1);  
//  attachInterrupt(1, ISRTHRO_end, FALLING);
//}
//
//void ISRTHRO_end() {
// servoTHRO_val = micros() - servoTHRO_start;
// detachInterrupt(1);
// attachInterrupt(1, ISRTHRO_begin, RISING);  
//}


