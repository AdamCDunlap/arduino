#ifndef SOFTBLINK_H
#define SOFTBLINK_H

#include <Arduino.h>

class SoftBlink {

public:
    SoftBlink() : pinNum(-1), fadeTime(0) {
    }
    SoftBlink(int inpinNum) : pinNum(inpinNum), fadeTime(0) {
    }
    SoftBlink(int inpinNum, unsigned int infadeTime) : pinNum(inpinNum), fadeTime(infadeTime) {
    }
    void setPin(int inpinNum) {
        pinNum = inpinNum;
    }
    void setFadeTime(unsigned int infadeTime) {
        fadeTime = infadeTime;
    } 
    void write(bool state) {
        if (state != lastState) {
            lastState = state;
            changeTime = millis();
        }
        analogWrite(pinNum, constrain(map(millis() - changeTime, 0, fadeTime, state? 0:255, state? 255:0), 0, 255));
    }

private:
    int pinNum;
    unsigned int fadeTime;
    bool lastState;
    unsigned long changeTime;
};

#endif
