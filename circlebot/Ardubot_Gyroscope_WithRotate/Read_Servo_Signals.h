#include <WProgram.h> //this contains all the arduino functions like digitalRead()

#define ORgate //comment out this line to not use the OR gate method
#define twoChannel //comment out to use one interrupt pin.

#define discardOldValues

volatile int pulseTime1; //AILE RX
volatile int pulseTime2; //THRO LY
volatile int pulseTime3; //ELEV RY
volatile int pulseTime4; //RUDD LX
volatile unsigned int pulseStart;

int interrupt_pin1 = 2;
int interrupt_pin2 = 3;

#ifdef ORgate //easy way to comment out code: just uncomment out "#define ORgate" above to get rid of this code and enable other half

/* Hardware Setup:
 * Each receiver output is attached to a digital pin
 * There is a diode between each receiver output and an external interrupt pin, with the anode on the interrupt side
 * This creates a logical OR, so the processPulse function will run when any of the pulses happen
 * It will then decide whether it was going high or low, then which pin it was, and set the aproperate variables
 
 * Call "pulseTime*()" to get pulse times in microseconds
 * Must call readReceiverBegin(powerPin, interruptPin1, interruptPin2 (optional), for second method) in setup()
 */
 
/*Using unsigned ints (0 - 64535) means that it will roll over every 64ms, but
the subtraction works out so that it's a "clean" rollover - it still gives
good values. For example, 10 - 64525 (maximum unsigned int - 10) = 21. Bytes
(0-255) could not be used since it would roll over many times during one pulse.
*/

#ifdef debugMode
volatile unsigned int case1 = 0;
volatile unsigned int case2 = 0;
volatile unsigned int case3 = 0;
volatile unsigned int case4 = 0;
volatile unsigned int error = 0;
#endif

#ifdef twoChannel
void processPulse1(){ //activated on a changed pulse  
  unsigned int time = micros();
  volatile static unsigned int lastPulseStart = 0;                                    
  static volatile byte currentPulse = 0;
  if (digitalRead(interrupt_pin1)){ //was it rising edge?
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
      case 0: //throttle
        pulseTime2 = time - pulseStart;
        #ifdef debugMode
          case2++;
        #endif
        break;
      case 1: //rudder
        pulseTime4 = time - pulseStart;
        #ifdef debugMode
          case4++;
        #endif
        break;
      #ifdef debugMode
      default: //asf
        error++;
        break;
      #endif
    }
  }
  lastPulseStart = pulseStart;
}
void processPulse2(){ //activated on a changed pulse
  unsigned int time = micros();
  volatile static unsigned int lastPulseStart = 0;                                    
  volatile static byte currentPulse = 0;
  if (digitalRead(interrupt_pin2)){ //was it rising edge?
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
      case 0: //aileron
        pulseTime1 = time - pulseStart;
        #ifdef debugMode
          case1++;
        #endif
        break;
      case 1: //elevation
        pulseTime3 = time - pulseStart;
        #ifdef debugMode
          case3++;
        #endif
        break;
      #ifdef debugMode
      default://asdf
        error++;
        break;
      #endif
    }
  }
  lastPulseStart = pulseStart;
}

#else //twoChannel

void processPulse(){ //activated on a changed pulse
  unsigned int time = micros();
  volatile static unsigned int lastPulseStart = 0;                                    
  volatile byte currentPulse = 0;
  if (digitalRead(interrupt_pin1)){ //was it rising edge?
    pulseStart = time;
    if (pulseStart - lastPulseStart > 2000){
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
          case0++;
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
#endif //twoChannel

#else //ORgate
#undef debugMode //we can only send back debug data when using the ORgate method.
/* Hardware Setup:
 * Attach each receiver signal wire to an external interrupt
 * Call "pulsetime*()" to get pulse times in microseconds
 */

/* This section of code is with no OR gate and each signal on its own interrupt.
 * It also documents how to do the interupts using RISING and FALLING rather than CHANGE.
 */
 
//unsigned long pulseStart = 0;
 
void ISRRUTT_end();

void ISRRUTT_begin() {
  pulseStart = micros();
  detachInterrupt(0);
  attachInterrupt(0, ISRRUTT_end, FALLING); //start function ISRRUTT_end when pin 2 goes back down
}

void ISRRUTT_end() {
 pulseTime4 = micros() - pulseStart;
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
  #ifdef twoChannel
  attachInterrupt(interruptPin1 - 2, processPulse1, CHANGE);
  attachInterrupt(interruptPin2 - 2, processPulse2, CHANGE);
  #else //twoChannel
  attachInterrupt(interruptPin1 - 2, processPulse, CHANGE);  
  #endif //twoChannel
  #else //ORgate
  attachInterrupt(interruptPin1 - 2, ISRRUTT_begin, RISING);
  attachInterrupt(interruptPin2 - 2, ISRTHRO_begin, RISING);
  #endif//ORgate
  interrupt_pin1 = interruptPin1;
  interrupt_pin2 = interruptPin2;
}

const unsigned long timeoutTime = 1000000; //throw away old values after some time

unsigned int joyTimeRX(void){
  #ifdef discardOldValues
  if ((unsigned int)micros() - pulseStart < timeoutTime)
  #endif //discardOldValues
    {return pulseTime1;}
  #ifdef discardOldValues
  else
    {return 0;}
  #endif //discardOldValues
}

unsigned int joyTimeLY(void){
  #ifdef discardOldValues
  if ((unsigned int)micros() - pulseStart < timeoutTime)
  #endif //discardOldValues
    {return pulseTime2;}
  #ifdef discardOldValues
  else
    {return 0;}
  #endif //discardOldValues
}

unsigned int joyTimeRY(void){
  #ifdef discardOldValues
  if ((unsigned int)micros() - pulseStart < timeoutTime)
  #endif //discardOldValues
    {return pulseTime3;}
  #ifdef discardOldValues
  else
    {return 0;}
  #endif //discardOldValues
}

unsigned int joyTimeLX(void){
  #ifdef discardOldValues
  if ((unsigned int)micros() - pulseStart < timeoutTime)
  #endif //discardOldValues
    {return pulseTime4;}
  #ifdef discardOldValues
  else
    {return 0;}
  #endif //discardOldValues
}

