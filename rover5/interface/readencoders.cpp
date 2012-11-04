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


#define ASM_INT_VECT_START(bit,encnum,breakif) asm volatile (                \
        "push r18\n\t" /*Read in the port ASAP*/                             \
        "in r18,%[pin]\n\t"                                                  \
                                                                             \
        "push r19\n\t" /*Use r19 to store and then push sreg*/               \
        "in r19,__SREG__\n\t"                                                \
        "push r19\n\t"                                                       \
        "push r24\n\t"                                                       \
        "push r25\n\t"                                                       \
        "push r26\n\t"                                                       \
        "push r27\n\t"                                                       \
                                                                             \
        "lds r24,%[dist]+0\n\t" /* Read [dist]*/                             \
        "lds r25,%[dist]+1\n\t"                                              \
        "lds r26,%[dist]+2\n\t"                                              \
        "lds r27,%[dist]+3\n\t"                                              \
                                                                             \
        "lds r19,%[lasta]\n\t"                                               \
        "eor r19,r18\n\t"                                                    \
                                                                             \
        "sbr"#breakif" r19,"#bit"\n\t"                                       \
        "rjmp .MYA"#encnum"\n\t"                                             \
                                                                             \
        "adiw r24,1\n\t" /* Add here*/                                       \
        "adc r26,__zero_reg__\n\t"                                           \
        "adc r27,__zero_reg__\n\t"                                           \
        "rjmp .MYB"#encnum"\n\t"                                             \
                                                                             \
        ".MYA"#encnum": sbiw r24,1\n\t" /* subtract here*/                   \
        "sbc r26,__zero_reg__\n\t"                                           \
        "sbc r27,__zero_reg__\n\t"                                           \
                                                                             \
        ".MYB"#encnum": sts %[dist]+0,r24\n\t" /* Store [dist]*/             \
        "sts %[dist]+1,r25\n\t"                                              \
        "sts %[dist]+2,r26\n\t"                                              \
        "sts %[dist]+3,r27\n\t"                                              
                                                                             
#define ASM_INT_VECT_END(port,distnum)                                       \
        "sts %[lasta],r18\n\t" /* don't bother getting rid of other bits;*/  \
                               /* we don't care about them*/                 \
                                                                             \
        "pop r27\n\t" /* Pop the registers back*/                            \
        "pop r26\n\t"                                                        \
        "pop r25\n\t"                                                        \
        "pop r24\n\t"                                                        \
        "pop r19\n\t"                                                        \
        "out __SREG__,r19\n\t"                                               \
        "pop r19\n\t"                                                        \
        "pop r18\n\t"                                                        \
        : /* No return stuff */                                              \
        : [lasta] "p" (&lastA)                                               \
        , [pin]   "I" (_SFR_IO_ADDR(port))                                   \
        , [dist]  "p" (&encdists[distnum]));                                       

// Encoder 3 interrupt vector
#ifdef USE_ASSEMBLER
ISR(PCINT0_vect, ISR_NAKED) {
#else
ISR(PCINT0_vect) {
#endif
// START PCINT0_vect
    static uint8_t lastA = false;

#ifdef USE_ASSEMBLER
    ASM_INT_VECT_START(0,2,s)
    "swap r18\n\t" /* Put the 4th byte in the low byte of [lasta]. */    
    ASM_INT_VECT_END(PINB,2)
#elif defined USE_OPTIMIZED
    uint8_t pinb = PINB;
    
    // Want bit 0 and 4 -- 4 goes into lastA, 0 is used for exor
    //  (not that that is different from below, so we'll decrement
    //   rather than increment and vice verse)
    if ((pinb ^ lastA) & _BV(0)) encdists[2]++;
    else                      encdists[2]--;
    lastA = pinb >> 4;
#else
    bool curA = !!(PINB & _BV(0));
    bool curB = !!(PINB & _BV(4));

    if (curA != lastA) encdists[2] += (curA^curB)? +1 : -1;
    else               encdists[2] += (curA^curB)? -1 : +1;

    lastA = curA;
#endif
// END PCINT0_vect
}

// Encoder 4 interrupt vector
#ifdef USE_ASSEMBLER
ISR(PCINT1_vect, ISR_NAKED) {
#else
ISR(PCINT1_vect) {
#endif
    static bool lastA = false;

#ifdef USE_ASSEMBLER
    ASM_INT_VECT_START(0,3,c)
    "swap r18\n\t" /* Put the 4th byte in the low byte of [lasta]. */    
    ASM_INT_VECT_END(PINC,3)
#elif defined USE_OPTIMIZED
    uint8_t pinc = PINC;
    
    // Want bit 0 and 1 -- 1 goes into lastA, 0 is used for exor
    //  (not that that is different from below, so we'll decrement
    //   rather than increment and vice verse)
    if ((pinc ^ lastA) & _BV(0)) encdists[3]++;
    else                      encdists[3]--;
    lastA = pinc >> 1;
#else
    bool curA = !!(PINC & _BV(0));
    bool curB = !!(PINC & _BV(1));

    if (curA != lastA) encdists[3] += (curA^curB)? +1 : -1;
    else               encdists[3] += (curA^curB)? -1 : +1;

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
    bool curA = !!(PIND & _BV(4));
    bool curB = !!(PIND & _BV(7));

    if (curA != lastA) encdists[1] += (curA^curB)? +1 : -1;
    else               encdists[1] += (curA^curB)? -1 : +1;

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
    bool curA = !!(PIND & _BV(2));
    bool curB = !!(PIND & _BV(3));

    encdists[0] += (curA^curB)? +1 : -1;
#endif
}
// Channel B interrupt
ISR(INT1_vect) {
#ifdef USE_OPTIMIZED
    uint8_t pind = PIND;
    if (((pind>>1) ^ (pind)) & _BV(2)) encdists[0] --;
    else                               encdists[0] ++;
#else
    bool curA = !!(PIND & _BV(2));
    bool curB = !!(PIND & _BV(3));
    encdists[0] += (curA^curB)? -1 : +1;
#endif
}

void i2cRequest() {
    Wire.write((uint8_t *)encdists, 16);
}
