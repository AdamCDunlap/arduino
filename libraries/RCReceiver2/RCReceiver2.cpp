/* RCReceiver.cpp - A library intended to get the times of the pulses
    from a remote control receiver designed to work directly with Servos.
 * Written by Adam Dunlap, July 2011
 * Released into the public domain.
*/

#define LIB_EXTERN
#include "RCReceiver2.h"

//#define DISCARD_OLD_VALUES

using namespace RCReceiver;

void RCReceiverBegin (uint8_t _interruptPin){
	interruptNumber = pinToInterrupt(_interruptPin);
	attachInterrupt(interruptNumber, processPulse, RISING);
	interruptPin = _interruptPin;
}

void RCReceiver::processPulse(){
	#if 0
	volatile unsigned int time = micros();
	volatile static uint8_t currentPulse = 0;
	pulseTimes[currentPulse] = time - pulseStart;
	#endif
	risingFalling = digitalRead(interruptPin);
	changed = true;
}

uint8_t RCReceiver::pinToInterrupt(uint8_t pin){
/*returns the interrupt number associated with a pin - works on mega and standard arduinos*/
/* 2 -> 0; 3 -> 1; 21 -> 2; 20 -> 3; 19 -> 4; 18 -> 5; -- from datasheet*/
        if (pin == 2 || pin == 3)
                return pin - 2;
        #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        else if (pin >= 18 || pin <= 21) //mega only
                return 7 - (pin - 16);
        #endif
        else{ //error
                return 255;
        }
}		
