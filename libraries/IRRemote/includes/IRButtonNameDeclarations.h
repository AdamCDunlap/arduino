#ifndef IRButtonNameDeclarations_h
#define IRButtonNameDeclarations_h

#include <avr/pgmspace.h>

const int numberOfCodes = 3;
const int maxCodeLength = 71;

extern const unsigned int irCodes[];

typedef enum {noButton,
    enter,
    down,
    play,
} irButtonNames;

#endif
