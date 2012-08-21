#ifndef ROVER5_H
#define ROVER5_H

#include <Arduino.h>
#include <Wire.h>

class Rover5 {
public:
    Rover5(uint8_t i2caddress = 12);

    void begin();

    enum mtrNum_t { BR = 0, FR = 1, BL = 2, FL = 3 };

    // Each power is an int from -255 to 255
    void Run(int frontLeft, int frontRight, int backLeft, int backRight);
    void Run(int powers[4]);
    void Run(int x, int y, int z = 0);
    void Run(int power, mtrNum_t num);

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
    // Returns true if speeds were updated or false if nothing was received
    //  over i2c yet
    bool UpdateSpeeds();
    
    // Sets the current i2c address
    void SetI2CAddress(uint8_t address) { interfaceAddress = address; }

private:
    uint8_t interfaceAddress;

    long ticks[4];

    int speeds[4];

    int powers[4];

    // Circular buffer class holding the last ten tick counts and times
    template <size_t bufsz> struct TickLogs {
        long ticks[bufsz][4];
        unsigned long times[bufsz];
        uint8_t nextEntry;

        TickLogs() : nextEntry(0) {}

        void Put(long iticks[4], unsigned long time) {
            memcpy(ticks[nextEntry], iticks, sizeof(ticks[0]));
            times[nextEntry] = time;

            nextEntry = (nextEntry >= bufsz-1)? 0 : nextEntry+1;
        }
    };

    static const size_t spdLogLen = 10;
    TickLogs<spdLogLen> tickLogs;

    // Scales array down to be under maximum but the same relative to
    //  each other
    void Normalize4(int nums[4], int maximum);
    // Runs motors using values stored in powers array
    void Run();

};

#endif // ROVER5_H
