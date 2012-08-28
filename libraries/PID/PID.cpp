#include <PID.h>

// Constructor -- optionally takes gains
PID::PID(internal_t outMin, internal_t outMax, gain_t inP, gain_t inI, gain_t inD)
    : kP(inP), kI(inI), kD(inD)
    , errorSum(0)
    , lastError(0)
    , outputMin(outMin)
    , outputMax(outMax)
    {
}

// The main method to call 
// Takes the sensor value and it returns the power
internal_t PID::GetOutput(internal_t cur, internal_t setpoint, unsigned long time) {
    internal_t error = setpoint - cur;

    printf_P(PSTR("PID ErrSum: %0f "), errorSum);
    
    internal_t Pval = kP * error;
    internal_t Dval = kD * ((error - lastError) / (time - lasttime));


    internal_t Ival;
    errorSum += kI * error * (time - lasttime);
    if (errorSum > outputMax) {
        errorSum = outputMax;
        Ival = outputMax;
    } else if (errorSum < outputMin) {
        errorSum = outputMin;
        Ival = outputMin;
    } else {
        Ival = kI * errorSum;
    }

    lasttime = time;
    lastError = error;

    internal_t output = Pval + Ival + Dval;
    return (output > outputMax)? outputMax : ((output < outputMin)? outputMin : output);
}

// Sets the PID gains
void PID::SetPID(gain_t inP, gain_t inI, gain_t inD) {
    kP = inP;
    kI = inI;
    kD = inD;
}

void PID::SetMinMax(internal_t outMin, internal_t outMax) {
    outputMin = outMin;
    outputMax = outMax;
}
