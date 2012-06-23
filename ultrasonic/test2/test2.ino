#include <Arduino.h>

//volatile uint8_t& USport = PORTD;
//volatile uint8_t& USpin  = PIND;
//volatile uint8_t& USddr  = DDRD;
//volatile uint8_t  USbit  = _BV(4);
#define DEBUG_PRINT(x) do {Serial.print(#x ": "); Serial.print(x); Serial.print('\t');} while(0)
uint8_t USpin = 4;

void setup();
void loop();
void manageUltrasonic(const byte USpin, unsigned int* distance);

void setup() {
    Serial.begin(115200);
}

void loop() {
    static unsigned int distance; // in deciinches
    static unsigned int pulsetime;

    unsigned long currentTimeMs = millis();
    unsigned long currentTimeUs = micros();
    static unsigned long lastTimeUs = 0;

    unsigned long looptimeUs = currentTimeUs - lastTimeUs;

    manageUltrasonic(USpin, &distance);

    static unsigned long lastPrintTime = 0;
    if (millis() - lastPrintTime >= 1000) {
        //Serial.print("Time: ");   Serial.print(pulsetime); Serial.print('\t');
        //Serial.print("Distance"); Serial.print(distance);  Serial.print('\t');
        //DEBUG_PRINT(pulsetime);
        DEBUG_PRINT(distance);
        //DEBUG_PRINT(USstate);
        DEBUG_PRINT(looptimeUs);
        Serial.println();

        lastPrintTime = millis();
    }

    lastTimeUs = currentTimeUs;
}

void manageUltrasonic(const byte USpin, unsigned int* distance) {
    unsigned long currentTimeUs = micros();
    unsigned long currentTimeMs = millis();

    static unsigned long startOfStateTime = 0;
    static unsigned long USlastReadTime = 0;
    static enum { starting, sendingPulse, waitingForPulseStart,
           waitingForPulseEnd, waitingForCooloff } USstate = starting;
    switch (USstate) {
    case starting:
        pinMode(USpin, OUTPUT);
        digitalWrite(USpin, HIGH);

        startOfStateTime  = currentTimeUs;
        USstate = sendingPulse;
        break;
    case sendingPulse:
        delayMicroseconds(5);
        //if (currentTimeUs - startOfStateTime >= 5) {
            digitalWrite(USpin, LOW);
            pinMode(USpin, INPUT);

            startOfStateTime = currentTimeUs;
            USstate = waitingForPulseStart;
        //}
        break;
    case waitingForPulseStart:
        if (currentTimeUs - startOfStateTime >= 100000UL) {
            Serial.println("Error reading sensor: didn't receive pulse");
            USstate = waitingForCooloff;
        }
        if (digitalRead(USpin)) {
            startOfStateTime = currentTimeUs;
            USstate = waitingForPulseEnd;
        }
        break;
    case waitingForPulseEnd:
        if (!digitalRead(USpin)) {
            unsigned int pulsetime = currentTimeUs - startOfStateTime;
            *distance =  pulsetime / 15;
            
            pinMode(USpin, OUTPUT);
            digitalWrite(USpin, LOW);
            startOfStateTime = currentTimeUs;
            USstate = waitingForCooloff;
        }
        break;
    case waitingForCooloff:
        if (currentTimeMs - USlastReadTime >= 100) {
            USlastReadTime = currentTimeMs;
            USstate = starting;
            startOfStateTime = currentTimeUs;
        }
        break;
    }
}
