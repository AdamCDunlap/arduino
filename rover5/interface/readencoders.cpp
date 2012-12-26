#include "readencoders.h"

//#define USE_ASSEMBLER
//#define USE_OPTIMIZED

void i2cRequest();

long encdists[4] = {0, 0, 0, 0};

void readencoders::setup() {

    Wire.onRequest(i2cRequest);

    // Disable all interrupts
    cli();

    // PCINT0 and 4 (pins 8 and 12) can trigger interrupt
    PCMSK0 = _BV(PCINT0) | _BV(PCINT4);
    // PCINT8 and PCINT9 (pins A0 and A1) can trigger interrupt
    PCMSK1 = _BV(PCINT8) | _BV(PCINT9);
    // PCINT20 and 21 (pins 4 and 5) can trigger interrupt
    PCMSK2 = _BV(PCINT20) | _BV(PCINT21);

    // Configure INT0 and 1 to generate interrupt on level change
    EICRA  = _BV(ISC10) | _BV(ISC00);
    
    // Enable both external interrupts
    EIMSK  = _BV(INT0) | _BV(INT1);
    // Enable all pin change interrupts
    PCICR  = _BV(PCIE2) | _BV(PCIE1) | _BV(PCIE0);

    // Enable interrupts
    sei();

}

void readencoders::loop() {
}

// Encoder 3 interrupt vector
ISR(PCINT0_vect) {
// START PCINT0_vect
    static uint8_t lastA = false;

#if defined USE_OPTIMIZED
    uint8_t pinb = PINB;
    
    // Want bit 0 and 4 -- 4 goes into lastA, 0 is used for exor
    //  (not that that is different from below, so we'll decrement
    //   rather than increment and vice verse)
    if ((pinb ^ lastA) & _BV(0)) encdists[2]++;
    else                         encdists[2]--;
    lastA = pinb >> 4;
#else
    bool curA = !!(PINB & _BV(4));
    bool curB = !!(PINB & _BV(0));

    if (curA != lastA) encdists[1] += (curA^curB)? +1 : -1;
    else               encdists[1] += (curA^curB)? -1 : +1;

    lastA = curA;
#endif
// END PCINT0_vect
}

ISR(PCINT1_vect) {
    static bool lastA = false;

#if defined USE_OPTIMIZED
    uint8_t pinc = PINC;
    
    // Want bit 0 and 1 -- 1 goes into lastA, 0 is used for exor
    //  (not that that is different from below, so we'll decrement
    //   rather than increment and vice verse)
    if ((pinc ^ lastA) & _BV(0)) encdists[3]++;
    else                         encdists[3]--;
    lastA = pinc >> 1;
#else
    bool curA = !!(PINC & _BV(0));
    bool curB = !!(PINC & _BV(1));

    if (curA != lastA) encdists[0] += (curA^curB)? +1 : -1;
    else               encdists[0] += (curA^curB)? -1 : +1;

    lastA = curA;
#endif
}

// Encoder 2 interrupt vector
ISR(PCINT2_vect) {
    static uint8_t lastA = false;
#ifdef USE_OPTIMIZED
    uint8_t pinc = PINC;
    
    // Want bits 4 and 7 -- 7 goes into lastA, 4 is used for exor
    //  (not that that is different from below, so we'll decrement
    //   rather than increment and vice verse)
    if ((pinc ^ lastA) & _BV(4)) encdists[1]++;
    else                         encdists[1]--;
    lastA = pinc >> 3;
#else
    bool curA = !!(PIND & _BV(5));
    bool curB = !!(PIND & _BV(4));

    if (curA != lastA) encdists[2] += (curA^curB)? +1 : -1;
    else               encdists[2] += (curA^curB)? -1 : +1;

    lastA = curA;
#endif
}

// Encoder 1 interrupt vectors
// Channel A interrupt
ISR(INT0_vect) {
#ifdef USE_OPTIMIZED
    uint8_t pind = PIND;
    if (((pind>>1) ^ (pind)) & _BV(2)) encdists[0] ++;
    else                               encdists[0] --;
#else
    bool curA = !!(PIND & _BV(3));
    bool curB = !!(PIND & _BV(2));

    encdists[3] += (curA^curB)? +1 : -1;
#endif
}
// Channel B interrupt
ISR(INT1_vect) {
#ifdef USE_OPTIMIZED
    uint8_t pind = PIND;
    if (((pind>>1) ^ (pind)) & _BV(2)) encdists[0] --;
    else                               encdists[0] ++;
#else
    bool curA = !!(PIND & _BV(3));
    bool curB = !!(PIND & _BV(2));
    encdists[3] += (curA^curB)? -1 : +1;
#endif
}

void i2cRequest() {
    Wire.write((uint8_t *)encdists, 16);
}
