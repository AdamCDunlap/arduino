#include <Arduino.h>

//volatile uint8_t& USport = PORTD;
//volatile uint8_t& USpin  = PIND;
//volatile uint8_t& USddr  = DDRD;
//volatile uint8_t  USbit  = _BV(4);
#define DEBUG_PRINT(x) do {Serial.print(#x ": "); Serial.print(x); Serial.print('\t');} while(0)
uint8_t USpin = 4;

void setup();
void loop();

void setup() {
    Serial.begin(115200);
}

void loop() {
    static unsigned int distance; // in cm
    static unsigned int pulsetime;

    unsigned long currentTimeMs = millis();
    unsigned long currentTimeUs = micros();
    // Used for storing a time, depending on state
    static unsigned long UStimePH = 0;
    static unsigned long USlastReadTime = 0;
    static enum { starting, sendingPulse, waitingForPulseStart,
           waitingForPulseEnd, waitingForCooloff } USstate = starting;
    switch (USstate) {
    case starting:
        pinMode(USpin, OUTPUT);
        digitalWrite(USpin, HIGH);

        UStimePH  = currentTimeUs;
        USstate = sendingPulse;
        break;
    case sendingPulse:
        if (currentTimeUs - UStimePH >= 5) {
            digitalWrite(USpin, LOW);
            pinMode(USpin, INPUT);

            UStimePH = currentTimeUs;
            USstate = waitingForPulseStart;
        }
        break;
    case waitingForPulseStart:
        //if (currentTimeUs - UStimePH >= 1000000UL) {
        //    Serial.println("Error reading sensor: didn't receive pulse");
        //    USstate = waitingForCooloff;
        //}
        if (digitalRead(USpin)) {
            UStimePH = currentTimeUs;
            USstate = waitingForPulseEnd;
        }
        break;
    case waitingForPulseEnd:
        if (!digitalRead(USpin)) {
            pulsetime = currentTimeUs - UStimePH;
            distance =  pulsetime / 58;
            
            pinMode(USpin, OUTPUT);
            digitalWrite(USpin, LOW);
            UStimePH = currentTimeUs;
            USstate = waitingForCooloff;
        }
        break;
    case waitingForCooloff:
        if (currentTimeMs - USlastReadTime >= 100) {
            USlastReadTime = currentTimeMs;
            USstate = starting;
            UStimePH = currentTimeUs;
        }
        break;
    }

    static unsigned long lastPrintTime = 0;
    if (millis() - lastPrintTime >= 1000) {
        //Serial.print("Time: ");   Serial.print(pulsetime); Serial.print('\t');
        //Serial.print("Distance"); Serial.print(distance);  Serial.print('\t');
        DEBUG_PRINT(pulsetime);
        DEBUG_PRINT(distance);
        DEBUG_PRINT(USstate);
        Serial.println();

        lastPrintTime = millis();
    }
}
