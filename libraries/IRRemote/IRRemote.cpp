#include "IRRemote.h"
#include "includes/IRButtonNameDefinitions.h"

volatile unsigned int receivedCode[maxCodeLength];
volatile int receivedNewMessage;

void processPulse(); // Function that the interrupt handler calls. TODO: make static member of class
void irInit(uint8_t);
inline uint8_t pinToInterrupt(uint8_t);

void irInit(uint8_t interruptPinNum) {
    pinMode(interruptPinNum, INPUT);
    attachInterrupt(pinToInterrupt(interruptPinNum), processPulse, CHANGE);
}

void processPulse() {
    unsigned long currentTime_us = micros();
    unsigned long currentTime_ms = millis();

    static unsigned long lastTime_us = 0; // Relative time of start of last loop in microseconds
    // Time it in both micro- and milli- seconds; microseconds for accuracy, milliseconds
    //  so it doesn't roll over more than once between button presses -- if someone were to wait
    //  2^32 - 1 + 0 to 500 milliseconds, it would register as a continuation of the same button
    //  ress.

    static unsigned long lastTime_ms = 0; // Relative time of start of last loop in milliseconds
    static unsigned char currentPulse = 0; // The next loop around with make it one, the real starting point
    if (currentTime_ms - lastTime_ms > 500){ // The end of the code has happened...
        //This is what will happen on the first pin change of the code
        receivedNewMessage = true;
        currentPulse = 0;
    } 
    
    else if (currentPulse > maxCodeLength) return; //Don't write beyond end of the array!

    else {
        // Add the time since the last call to the current spot in the pulseTimes array
        receivedCode[++currentPulse] = (currentTime_us - lastTime_us)/10;
    }
    receivedCode[0] = currentPulse; // Set the current length of the array
    
    lastTime_us = currentTime_us;
    lastTime_ms = currentTime_ms;
}

irButtonNames irGetButton() {
    if (receivedNewMessage) {
        const unsigned int* storedCodePtr = &irCodes[0]; //Make a temp pointer
        // const refers to the memory it's pointing to -- where it's pointing will change

        for (int i=0;i<numberOfCodes;i++){ //Loop through all the groups, checking each one
            // Default to having match, will use continue or make false when non-matching is confirmed
            bool Success = true;

            // The first element of each group is the length of the group
            //unsigned int storedCodeLength = pgm_read_word_near(storedCodePtr++);
            unsigned int storedCodeLength = *storedCodePtr++;

            // Calculate where the next group begins
            unsigned int* endpoint = (unsigned int*)storedCodePtr + storedCodeLength;
    
            // If the new signal is shorter than the original signal, it's not correct.
            // It could be longer because of repeat codes, etc.
            if (storedCodeLength > receivedCode[0]) {
                storedCodePtr = endpoint;
                continue;
            }
    
            for (unsigned int j = 1; j <= storedCodeLength; j++) {
                // If the difference between the two signals is more than 50%, return false
                // Cast to signed longs so we don't overflow in either direction
                //int dif = abs((long)receivedCode[j] - (long)pgm_read_word_near(storedCodePtr));
                unsigned int dif = abs((long)receivedCode[j] - (long)storedCodePtr);

                //if (dif > pgm_read_word_near(storedCodePtr) / 2) {
                if (dif > *storedCodePtr / 2) {
                    storedCodePtr = endpoint;
                    Success = false;
                    break;
                }
                // Increment the pointer and check the next value.
                storedCodePtr++;
            }
            
            // If we are here, the length check succeeded, but something didn't match
            // If it didn't match, reset the receivedNewMessage flag
            // We put it here so that if the length check succeeds, it won't reset the flag
            // This enables us to try again if we are halfway through the message
            receivedNewMessage = false;
            // If this loop found a match, return the name matching the current loop.
            if (Success){
                return (irButtonNames) (i + 1);
            } 
        } 
    }
    // We'll only get here if nothing matched or the receivedNewMessage flag was not set
    return (irButtonNames) 0; // No code
}

String irGetButtonName() {
    return irButtonNameStrings[irGetButton()];
}

//"private" "member" functions and variables.

inline uint8_t pinToInterrupt(uint8_t pin){
    /* Returns the interrupt number associated with a pin - works on Mega and standard Arduinos */
    /* 2 -> 0; 3 -> 1; 21 -> 2; 20 -> 3; 19 -> 4; 18 -> 5; -- from datasheet */
    if (pin == 2 || pin == 3)
        return pin - 2;
    #if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    else if (pin >= 18 && pin <= 21) //mega only
        return 23 - pin;
    #endif
    else{ //error
        return 255;
    }
}

