#include <PID.h>

// Constructor -- optionally takes gains
PID::PID(bool isInverted, internal_t outMin, internal_t outMax, gain_t inP, gain_t inI, gain_t inD)
    : errorSum(0)
    , lastInput(0)
    {
    SetInverted(isInverted);
    SetMinMax(outMin, outMax);
    SetPID(inP, inI, inD);
}

// The main method to call 
// Takes the sensor value and it returns the power
internal_t PID::GetOutput(internal_t cur, internal_t setpoint, unsigned long time) {
    internal_t error = setpoint - cur;

    
    internal_t Pval = kP * error;
    internal_t Dval = kD * ((lastInput - cur) / (time - lasttime));

    //printf_P(PSTR("ErrSum: %.2f Timediff: %lu  Inputdiff: %2.0f "), errorSum, time - lasttime, lastInput - cur);

    errorSum += kI * error * (time - lasttime);
         if (errorSum > outputMax) errorSum = outputMax;
    else if (errorSum < outputMin) errorSum = outputMin;
    internal_t Ival = errorSum;

    lasttime = time;
    lastInput = cur;

    internal_t output = Pval + Ival + Dval;
    return (output > outputMax)? outputMax : ((output < outputMin)? outputMin : output);
}

// Sets the PID gains
void PID::SetPID(gain_t inP, gain_t inI, gain_t inD) {
    if (inP<0 || inI <0 || inD<0) return;
    if (inverted) {
        kP = -inP;
        kI = -inI;
        kD = -inD;
    } else {
        kP = +inP;
        kI = +inI;
        kD = +inD;
    }
}

void PID::SetMinMax(internal_t outMin, internal_t outMax) {
    outputMin = outMin;
    outputMax = outMax;
}

void PID::SetInverted(bool isInverted) {
    inverted = isInverted;
    if (isInverted) {
        kP *= -1;
        kI *= -1;
        kD *= -1;
    }
}
