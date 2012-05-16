#ifndef RCReceiver_h
#define RCReceiver_h

#include "WProgram.h"

#define NUMBER_OF_CHANNELS 4 //make larger depending on how many channels your receiver has

void RCReceiverBegin(uint8_t /*interruptPin1*/, uint8_t /*interruptPin2*/);
unsigned int pulseTime(unsigned int which, long timeoutTime);

//namespace RCReceiver {
	void processPulse1(void);
	void processPulse2(void);
	volatile int pulseTimes[NUMBER_OF_CHANNELS];
	volatile unsigned int pulseStart;
	uint8_t pinToInterrupt(uint8_t pin);
	uint8_t interruptPin1;
	uint8_t interruptPin2;
	uint8_t firstIntr;
	uint8_t secondIntr;
//}
#endif //RCReceiver_h

/* 
 * RCReceiver.cpp - A library intended to get the times of the pulses
    from a remote control receiver designed to work directly with Servos.
 * Written by Adam Dunlap, June 2011
 * Released into the public domain.
*/

//r#include "RCReceiver.h"
#include "WProgram.h"

//#define DISCARD_OLD_VALUES

//using namespace RCReceiver;

void RCReceiverBegin (uint8_t firstIntPin, uint8_t secondIntPin){
	firstIntr = pinToInterrupt(firstIntPin);
	secondIntr = pinToInterrupt(secondIntPin);
	attachInterrupt(firstIntr, processPulse1, CHANGE);
	attachInterrupt(secondIntr, processPulse2, CHANGE);
	interruptPin1 = firstIntPin;
	interruptPin2 = secondIntPin;
}

void processPulse1(){ //activated on a changed pulse  
	unsigned int time = micros();
	volatile static unsigned int lastPulseStart = 0;                                    
	static volatile uint8_t currentPulse = 0;
	if (digitalRead(interruptPin1)){ //was it rising edge?
		pulseStart = time;
		if (pulseStart - lastPulseStart > 10000){
			currentPulse = 1;
		}
		else{
			currentPulse += 2;
		}
		lastPulseStart = pulseStart; //probably should be here; try once library is confirmed working
	}
	else{ //pulse was falling edge
		#if 0
		switch(currentPulse){ //which pulse is this the end of?
			case 0:
				pulseTime2 = time - pulseStart;
				break;
			case 1:
				pulseTime4 = time - pulseStart;
				break;
		}
//		#else
		#endif
		pulseTimes[currentPulse] = time - pulseStart;
//		#endif
	}
//	lastPulseStart = pulseStart; //probably should not be here.
}
void processPulse2(){ //activated on a changed pulse
	unsigned int time = micros();
	volatile static unsigned int lastPulseStart = 0;                                    
	volatile static uint8_t currentPulse = 0;
	if (digitalRead(interruptPin2)){ //was it rising edge?
		pulseStart = time;
		if (pulseStart - lastPulseStart > 10000){
			currentPulse = 0;
		}
		else{
			currentPulse += 2;
		}
		lastPulseStart = pulseStart; //should be here; try once library is complete
	}
	else{ //pulse was falling edge
		#if 0
		switch(currentPulse){ //which pulse is this the end of?
 			case 0:
    				pulseTime1 = time - pulseStart;
				break;
			case 1:
				pulseTime3 = time - pulseStart;
				break;
		}
//		#else
		#endif
		pulseTimes[currentPulse] = time - pulseStart;
//		#endif
	}
//	lastPulseStart = pulseStart; //probably should not be here
}

unsigned int pulseTime (unsigned int which, unsigned long timeoutTime){
	#ifdef DISCARD_OLD_VALUES
	if ((unsigned int)millis() - (pulseStart/1000) < timeoutTime)
	#endif // DISCARD_OLD_VALUES
		return pulseTimes[which];
	#ifdef DISCARD_OLD_VALUES
	else
		return 0;
	#endif // DISCARD_OLD_VALUES
}


uint8_t pinToInterrupt(uint8_t pin){
/*returns the interrupt number associated with a pin - works on mega and standard arduinos*/
/* 2 -> 0; 3 -> 1; 21 -> 2; 20 -> 3; 19 -> 4; 18 -> 5; -- from datasheet*/
	if (pin == 2 || pin ==3)
		return pin - 2;
	#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	else if (pin >= 18 || pin <= 21) //mega only
		return 7 - (pin - 16);
	#endif
	else{ //error
		return 255;
	}
}

void setup(){
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Serial.begin(115200);
  Serial.println("Starting");
  RCReceiverBegin(2, 3);
  Serial.println("Receiver Begun");
}

void loop(){
  for (unsigned int i=0;i<4;i++){
    Serial.print(pulseTime(i, 100000U));
    Serial.print("\t");
  }
  Serial.println();
  delay(50);
}
