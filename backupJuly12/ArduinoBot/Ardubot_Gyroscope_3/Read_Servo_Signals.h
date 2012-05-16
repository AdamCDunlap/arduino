#include <WProgram.h> //this contains all the arduino functions like digitalRead()

//#define ORgate //comment out this line to not use the OR gate method

volatile unsigned int servoRUTT_start; //Using unsigned ints (0 - 64535) means that
volatile unsigned int servoRUTT_val;   //it will roll over every 64ms, but the
volatile unsigned int servoTHRO_start; //subtraction works out so that it's a 
volatile unsigned int servoTHRO_val;   //"clean" rollover - For example,
volatile unsigned int servoAILE_start; //10 - 64525 = 20. Bytes (0-255) could not be used
volatile unsigned int servoAILE_val;   //since it would roll over many times in one pulse

#ifdef ORgate //easy way to comment out code: just uncomment out "#define ORgate" above to get rid of this code

/* Hardware Setup:
 * Each receiver output is attached to a digital pin
 * There is a diode between each receiver output and an external interrupt pin, with the anode on the interrupt side
 * This creates a logical OR, so the processPulse function will run when any of the pulses happen
 * It will then decide whether it was going high or low, then which pin it was, and set the aproperate variables
 
 * Call "servo****_val_f()" to get pulse times in microseconds
 * Must call attachInterrupt(processPulse, *, CHANGE) where * is the interrupt pin (0 or 1)
 */
 
// class RCPulses{
//   public:
//   RCPulses(int interrupt_pin, int servoRUTT_pin, int servoTHRO_pin, int servoAILE_pin)

const int interrupt_pin = 2;
const int servoRUTT_pin = 3; //named RUTT, THRO, and AILE because that's what's printed
const int servoTHRO_pin = 9; //on my receiver.
const int servoAILE_pin = 10;

                                       //These variables  store the starting times of pulses
                                       //and how long they lasted.
                                       
volatile byte currentPulse;            //this variable stores the value of the current pulse.
                                       //Always use volatile variables when working with interrupts.

void processPulse(){ //activated on a changed pulse  
  if (digitalRead(interrupt_pin)){ //was it rising edge?
    if (digitalRead(servoRUTT_pin)){ //it's a rising edge - which port was it?
      currentPulse = 0; //tells which pulse happened for when it goes LOW
      servoRUTT_start = micros();
    }
    else if (digitalRead(servoTHRO_pin)){
      currentPulse = 1;
      servoTHRO_start = micros();
    }
    else /*if (digitalRead(servoAILE_pin))*/{ //the extra digitalRead takes more time, and we know it must be true at this point.
      currentPulse = 2;
      servoAILE_start = micros();
    }
  }
  else{ //pulse was falling edge
    switch(currentPulse){ //which pulse is this the end of?
      case 0: //done when currentPulse == 0
        servoRUTT_val = micros() - servoRUTT_start;
        break; //without break, it would just go on to the next case
      case 1: //when currentPulse == 1
        servoTHRO_val = micros() - servoTHRO_start;
        break;
      case 2: //...etc
        servoAILE_val = micros() - servoAILE_start;
        break;
    }
  }
}

#endif

#ifndef ORgate //easy way to "comment out" code - just comment out "#define ORgate" above.
/* Hardware Setup:
 * Attach each receiver signal wire to an external interrupt
 * Call "servo****_val_f()" to get pulse times in microseconds
 */

/* This section of code is with no OR gate and each signal on its own interrupt.
 * It also documents how to do the interupts using RISING and FALLING rather than CHANGE.
 */


void ISRRUTT_end();

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
#endif

const unsigned int timeoutTime = 50000; //be careful updating this value; max. int is 65,535

//commented out code should work, but has not been tested
//this section of code returns the values to the other program
//pretty useless for now, since they're all global varialbes

unsigned int servoRUTT_val_f(void){
//  if (micros() - servoRUTT_start < timeoutTime){  //throw away old values after some time
    return servoRUTT_val;
//  }
//  else{
//    return 0;
//  }
}

unsigned int servoTHRO_val_f(void){
//  if (micros() - servoTHRO_start < timeoutTime){
    return servoTHRO_val;
//  }
//  else{
//    return 0;
//  }
}

unsigned int servoAILE_val_f(void){
//  if (micros() - servoAILE_start < timeoutTime){ 
    return servoAILE_val;
//  }
//  else{
//    return 0;
//  }
}

