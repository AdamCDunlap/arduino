#include "readencoders.h"

void i2cRequest();

unsigned int encdists[4] = {0, 0, 0, 0};

void readencoders::setup() {

    Wire.onRequest(i2cRequest);

    // Disable all interrupts
    cli();

    // PCINT0 and 4 (pins 8 and 12) can trigger interrupt
    PCMSK0 = _BV(PCINT0) | _BV(PCINT4);
    // PCINT8 and PCINT9 (pins A0 and A1) can trigger interrupt
    PCMSK1 = _BV(PCINT8) | _BV(PCINT9);
    // PCINT20 and 23 (pins 4 and 7) can trigger interrupt
    PCMSK2 = _BV(PCINT20) | _BV(PCINT23);

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
    static bool lastA = false;
    bool curA = !!(PINB & _BV(0));
    bool curB = !!(PINB & _BV(4));

    if (curA != lastA) encdists[2] += (curA^curB)? +1 : -1;
    else               encdists[2] += (curA^curB)? -1 : +1;

    lastA = curA;
// END PCINT0_vect
}

// Encoder 4 interrupt vector
ISR(PCINT1_vect) {
    static bool lastA = false;
    bool curA = !!(PINC & _BV(0));
    bool curB = !!(PINC & _BV(1));

    if (curA != lastA) encdists[3] += (curA^curB)? +1 : -1;
    else               encdists[3] += (curA^curB)? -1 : +1;

    lastA = curA;
}

// Encoder 2 interrupt vector
ISR(PCINT2_vect) {
    static bool lastA = false;
    bool curA = !!(PIND & _BV(4));
    bool curB = !!(PIND & _BV(7));

    if (curA != lastA) encdists[1] += (curA^curB)? +1 : -1;
    else               encdists[1] += (curA^curB)? -1 : +1;

    lastA = curA;
}

// Encoder 1 interrupt vectors
// Channel A interrupt
ISR(INT0_vect) {
    //static bool lastA = false;
    bool curA = !!(PIND & _BV(2));
    bool curB = !!(PIND & _BV(3));

    //if (curA != lastA)
        encdists[0] += (curA^curB)? +1 : -1;
    //else
    //    encdists[0] += (curA^curB)? -1 : +1;

    //lastA = curA;
}
// Channel B interrupt
ISR(INT1_vect) {
    bool curA = !!(PIND & _BV(2));
    bool curB = !!(PIND & _BV(3));
    encdists[0] += (curA^curB)? -1 : +1;
}

void i2cRequest() {
    Wire.write((uint8_t *)&encdists[0], 2);
    Wire.write((uint8_t *)&encdists[1], 2);
    Wire.write((uint8_t *)&encdists[2], 2);
    Wire.write((uint8_t *)&encdists[3], 2);
}
