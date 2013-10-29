#include "GoldCode.h"
#include <limits.h>
#ifndef ARDUINO
#include <stdio.h>
#endif

// By Patrick McKeen and Adam Dunlap

uint32_t GoldCode::goldCode(int feedback1[], int feedback2[], int seed2) //returns the numerical value of the gold code
{
    uint32_t reg1=shiftRegister(1, feedback1);
    uint32_t reg2=shiftRegister(seed2, feedback2);
    return reg1^reg2; // XORs the 2 outputs of the LFSRs together
}

uint32_t GoldCode::shiftRegister(uint32_t seed, int feedbackList[]) //finds the shift register for the LFSR
{
    uint32_t reg = 0;
    uint32_t b = seed;
    for (int i=0; (b-seed) != 0 || i == 0 ; i++) {
        reg |= (b&1L) << (30-i); //takes the rightmost digit of each iteration and adds it to the binary string
        b = nextStep(b, feedbackList); //iterates to the next step
    }
    return reg;
}

uint32_t GoldCode::nextStep(uint32_t a, int feedbackList[]) //finds the next value outpu of the LFSR
{
    uint32_t r= a >> 1;//shifts one place to the right
    bool addOne = false;
    for (int i=1; i<feedbackList[0];i++)
    {
        bool n = ((1L << (5 - feedbackList[i])) & a) != 0; //tests if the feedback terminals are outputting 1s
        addOne^=n; //XORs the outputs of the feedback terminals together
    }
    return (addOne << 4 ) | r; //generates next iteration
}

int GoldCode::dotProduct(uint32_t gc1, uint32_t gc2) {

    // From http://graphics.stanford.edu/~seander/bithacks.html

    uint32_t v = gc1 ^ ~gc2; // count bits set in this (32-bit value)
    uint32_t c; // store the total here
    
    c = v - ((v >> 1) & 0x55555555);
    c = ((c >> 2) & 0x33333333) + (c & 0x33333333);
    c = ((c >> 4) + c) & 0x0F0F0F0F;
    c = ((c >> 8) + c) & 0x00FF00FF;
    c = ((c >> 16) + c) & 0x0000FFFF;
    return c;
}

bool GoldCode::sameGC(uint32_t gc1, uint32_t gc2) {

    int best_score = -INT_MIN;
    for (int i=0; i<31; i++) {
        int score = dotProduct(gc1, gc2);
        gc2 = ((gc2 & 1) << 30) | gc2 >> 1;
        if (score > best_score) best_score = score;
    }
    return best_score > 25;
}


void GoldCode::printGC(uint32_t gc) {
    for (uint32_t i = 1 << 30; i != 0; i >>= 1) {
        int c = !!(i & gc) + '0';
#ifdef ARDUINO
        Serial.write(c);
#else
        putchar(c);
#endif
        
    }
}
