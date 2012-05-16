#ifndef RCReceiver_h
#define RCReceiver_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifndef LIB_EXTERN
#define LIB_EXTERN extern
#endif

#define NUMBER_OF_CHANNELS 4

void RCReceiverBegin(uint8_t interruptPin);
unsigned int pulseTime(int which, unsigned long timeoutTime);

LIB_EXTERN int risingFalling;
LIB_EXTERN int changed;

namespace RCReceiver{
	void processPulse();
	uint8_t pinToInterrupt(uint8_t pin);
	LIB_EXTERN volatile unsigned int pulseTimes[NUMBER_OF_CHANNELS];
	LIB_EXTERN int interruptNumber;
	LIB_EXTERN int interruptPin;
}

#endif //RCReceiver_h
