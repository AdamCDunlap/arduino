#ifndef PID_H
#define PID_H

#include <math.h>
#include <Arduino.h>

typedef double gain_t;
typedef double internal_t;
class PID {
public:
    // Constructor -- optionally takes gains and bounds
    PID(bool isInverted = false,
        internal_t outMin=-INFINITY, internal_t outMax=INFINITY,
        gain_t inP=0, gain_t inI=0, gain_t inD=0);

    // The main method to call 
    // Takes the sensor value and it returns the power
    internal_t GetOutput(internal_t cur, internal_t setpoint, unsigned long time);

    // Sets the PID gains
    void SetPID(gain_t inP, gain_t inI, gain_t inD);

    void SetMinMax(internal_t outMin, internal_t outMax);

    // Set to true if setting the output positive makes the input decrease
    void SetInverted(bool isInverted);
private:
    gain_t kP, kI, kD;
    bool inverted;

    internal_t errorSum;
    
    internal_t lastInput;

    internal_t outputMin, outputMax;
    unsigned long lasttime;
};

#endif
