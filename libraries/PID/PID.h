#ifndef PID_H
#define PID_H

template <class gain_t, class internal_t = gain_t>
class PID_base {
public:
    // Constructor -- optionally takes gains
    PID_base(gain_t inP=0, gain_t inI=0, gain_t inD=0);

    // The main method to call 
    // Takes the sensor value and it returns the power
    internal_t GetOutput(internal_t cur, unsigned long time);

    // Sets the PID gains
    void SetPID(gain_t inP, gain_t inI, gain_t inD);

    // Changes the setpoint
    void SetSetpoint(internal_t newsetpoint);

private:
    gain_t kP, kI, kD;

    internal_t setpoint;
    
    internal_t errorSum;
    
    internal_t lastError;

    unsigned long lasttime;
};

typedef PID_base<double, double> PID;

#endif
