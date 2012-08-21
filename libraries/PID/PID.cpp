#include <PID.h>

// Constructor -- optionally takes gains
template <class gain_t, class internal_t>
PID_base<gain_t, internal_t>::PID_base(gain_t inP, gain_t inI, gain_t inD)
    : kP(inP), kI(inI), kD(inD)
    , setpoint(0)
    , errorSum(0)
    , lastError(0)
    {
}

// The main method to call 
// Takes the sensor value and it returns the power
template <class gain_t, class internal_t>
internal_t PID_base<gain_t, internal_t>::GetOutput(internal_t cur, unsigned long time) {
    internal_t error = setpoint - cur;
    errorSum += error * (time - lasttime);
    
    internal_t Pval = kP * error;
    internal_t Ival = kI * errorSum;
    internal_t Dval = kD * ((error - lastError) / (time - lasttime));

    lasttime = time;

    return Pval + Ival + Dval;
}

// Sets the PID gains
template <class gain_t, class internal_t>
void PID_base<gain_t, internal_t>::SetPID(gain_t inP, gain_t inI, gain_t inD) {
    kP = inP;
    kI = inI;
    kD = inD;
}

// Changes the setpoint
template <class gain_t, class internal_t>
void PID_base<gain_t, internal_t>::SetSetpoint(internal_t newsetpoint) {
    setpoint  = newsetpoint;
}
