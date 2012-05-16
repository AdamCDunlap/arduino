#include <WProgram.h> //this contains all the arduino functions like digitalRead()

#define ORgate //comment out this line to not use the OR gate method

volatile unsigned int pulseTime1; //Using unsigned ints (0 - 64535) means that it will roll over every 64ms, but
volatile unsigned int pulseTime2; //the subtraction works out so that it's a "clean" rollover - it still gives
volatile unsigned int pulseTime3; //good values. For example, 10 - 64525 (maximum unsigned int - 10) = 20. Bytes
volatile unsigned int pulseTime4; //(0-255) could not be used since it would roll over many times during one pulse.

#ifdef ORgate //easy way to comment out code: just uncomment out "#define ORgate" above to get rid of this code and enable other half

/* Hardware Setup:
 * Each receiver output is attached to a digital pin
 * There is a diode between each receiver output and an external interrupt pin, with the anode on the interrupt side
 * This creates a logical OR, so the processPulse function will run when any of the pulses happen
 * It will then decide whether it was going high or low, then which pin it was, and set the aproperate variables
 
 * Call "pulseTime*()" to get pulse times in microseconds
 * Must call readReceiverBegin(powerPin, interruptPin1, interruptPin2 (optional), for second method) in setup()
 */

int interrupt_pin = 2;

volatile unsigned int pulseStart = 0;
volatile unsigned int lastPulseStart = 0;                                    
volatile byte currentPulse = 0;

#ifdef debugMode
volatile unsigned int case0 = 0;
volatile unsigned int case1 = 0;
volatile unsigned int case2 = 0;
volatile unsigned int case3 = 0;
volatile unsigned int error = 0;
#endif

void processPulse(){ //activated on a changed pulse
  unsigned int time = micros();
  if (digitalRead(interrupt_pin)){ //was it rising edge?
    pulseStart = time;
    if (pulseStart - lastPulseStart > 10000){
      currentPulse = 0;
    }
    else{
      currentPulse++;
    }
  }
  else{ //pulse was falling edge
    switch(currentPulse){ //which pulse is this the end of?
      case 0:
        pulseTime1 = time - pulseStart;
        #ifdef debugMode
          case1++;
        #endif
        break;
      case 1:
        pulseTime2 = time - pulseStart;
        #ifdef debugMode
          case1++;
        #endif
        break;
      case 2: //...etc
        pulseTime3 = time - pulseStart;
        #ifdef debugMode
          case2++;
        #endif
        break;
      case 3:
        pulseTime4 = time - pulseStart;
        #ifdef debugMode
          case3++;
        #endif
        break;
      #ifdef debugMode
      default:
        error++;
        break;
      #endif
    }
    lastPulseStart = pulseStart;
  }
}

#else //ORgate
/* Hardware Setup:
 * Attach each receiver signal wire to an external interrupt
 * Call "pulsetime*()" to get pulse times in microseconds
 */

/* This section of code is with no OR gate and each signal on its own interrupt.
 * It also documents how to do the interupts using RISING and FALLING rather than CHANGE.
 */
 
void ISRRUTT_end();

void ISRRUTT_begin() {
  pulseStart = micros();
  detachInterrupt(0);
  attachInterrupt(0, ISRRUTT_end, FALLING); //start function ISRRUTT_end when pin 2 goes back down
}

void ISRRUTT_end() {
 pulseTime1 = micros() - pulseStart;
 detachInterrupt(0);
 attachInterrupt(0, ISRRUTT_begin, RISING);
}

void ISRTHRO_end();

void ISRTHRO_begin() {
  pulseStart = micros();
  detachInterrupt(1);  
  attachInterrupt(1, ISRTHRO_end, FALLING);
}

void ISRTHRO_end() {
 pulseTime2 = micros() - pulseStart;
 detachInterrupt(1);
 attachInterrupt(1, ISRTHRO_begin, RISING);  
}
#endif //ORgate

void readReceiverBegin(int powerPin, int interruptPin1, int interruptPin2 = 0){
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, HIGH);
  #ifdef ORgate
  attachInterrupt(interruptPin1 - 2, processPulse, CHANGE);  
  #else
  attachInterrupt(interruptPin1 - 2, ISRRUTT_begin, RISING);
  attachInterrupt(interruptPin2 - 2, ISRTHRO_begin, RISING);
  #endif
  interrupt_pin = interruptPin1;
}

const unsigned long timeoutTime = 100000L; //throw away old values after some time

unsigned int joyTimeLX(void){
  /*if (micros() - long(pulseStart) < timeoutTime){*/return pulseTime1;/*}
  else{return 0;}*/
}

unsigned int joyTimeRY(void){
  /*if (micros() - long(pulseStart) < timeoutTime){*/return pulseTime2;/*}
  else{return 0;}*/
}

unsigned int joyTimeLY(void){
  /*if (micros() - long(pulseStart) < timeoutTime){*/return pulseTime3;/*}
  else{return 0;}*/
}

unsigned int joyTimeRX(void){
  /*if (micros() - long(pulseStart) < timeoutTime){*/return pulseTime4;/*}
  else{return 0;}*/
}

