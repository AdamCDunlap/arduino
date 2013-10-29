#ifndef GOLDCODE_H
#define GOLDCODE_H
#include <stdint.h>

// Orignally written by Patrick McKeen
// Modified by Adam Dunlap

namespace GoldCode {
    uint32_t goldCode(int feedback1[], int feedback2[], int seed2);
    uint32_t shiftRegister(uint32_t seed, int feedbackList[]);
    uint32_t nextStep(uint32_t a, int feedbackList[]);
    
    int dotProduct(uint32_t gc1, uint32_t gc2);
    bool sameGC(uint32_t gc1, uint32_t gc2);

    void printGC(uint32_t gc);
}

#endif//GOLDCODE_H
