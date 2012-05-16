#ifndef codes_pde
#define codes_pde

const unsigned int irCodes[] = {
    1, 5,
};

const char* irButtonNameStrings[] = {"noButton",
    "enter",
};

IRRemote::begin(int pin) {
    
