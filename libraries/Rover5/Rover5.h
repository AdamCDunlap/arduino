#ifndef ROVER5_H
#define ROVER5_H

#include <Arduino.h>
#include <Wire.h>

class Rover5 {
public:
    Rover5(uint8_t i2caddress = 12);

    void begin();

    enum mtrNum { BR = 0, FR = 1, BL = 2, FL = 3 };

    // Each speed is an int from -255 to 255
    void Run(int frontLeft, int frontRight, int backLeft, int backRight);
    void Run(int powers[4]);
    void Run(int x, int y, int z = 0);

    // Populates the powers array with the current speeds of each motor
    void GetPowers(int powers[4]);

    // Populates the ticks array with the current number of encoder ticks for
    //  each motor
    void GetTicks(long ticks[4]);

    // Populates the speeds array with the current speed of each motor in
    //  mills per second
    void GetSpeeds(int speeds[4]);
    
    // Read the current distances from the interface arduino and calculates
    //  the speeds
    void UpdateSpeeds();
    
    // Sets the current i2c address
    void SetI2CAddress(uint8_t address) { interfaceAddress = address; }

private:
    uint8_t interfaceAddress;

    long ticks[4];

    int speeds[4];

    int powers[4];

    // Circular buffer class holding the last ten tick counts and times
    template<uint8_t bufsz> struct TicksLog {
        long ticks[bufsz]; // Store last ten
        unsigned long times[bufsz];
        uint8_t  newestPos;
        uint8_t nextPos;

        TicksLog() :nextPos(0) {}
        void Put(long ct, unsigned long nexttime) {
            nextPos = (nextPos >= bufsz-1)? 0 : nextPos+1;
            ticks[nextPos] = ct;
            times[nextPos] = nexttime;
        }
    };
    TicksLog<10> tickLogs[4];

    // Scales array down to be under maximum but the same relative to
    //  each other
    void Normalize4(int nums[4], int maximum);
    // Runs motors using values stored in powers array
    void Run();

};

#endif // ROVER5_H
