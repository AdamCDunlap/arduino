#ifndef RCReceiver_h
#define RCReceiver_h

#include "WProgram.h"

#define NUMBER_OF_CHANNELS 4 //make larger depending on how many channels your receiver has

void RCReceiverBegin(uint8_t /*interruptPin1*/, uint8_t /*interruptPin2*/);
unsigned int pulseTime(int which, long timeoutTime);

#ifndef LIB_EXTERN
#define LIB_EXTERN extern //make the variables external except
#endif                    //in the RCReceiver.cpp file

//namespace RCReceiver {
	void processPulse1(void);
	void processPulse2(void);
	LIB_EXTERN volatile int pulseTimes[NUMBER_OF_CHANNELS];
	LIB_EXTERN volatile unsigned int pulseStart;
	uint8_t pinToInterrupt(uint8_t pin);
	LIB_EXTERN uint8_t interruptPin1;
	LIB_EXTERN uint8_t interruptPin2;
	LIB_EXTERN uint8_t firstIntr;
	LIB_EXTERN uint8_t secondIntr;
//}
#endif //RCReceiver_h
