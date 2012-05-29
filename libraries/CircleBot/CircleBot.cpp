#include "CircleBot.h"
#include <Arduino.h>

inline void* operator new (size_t, void* buf) { return buf; }
// Constructor
// Use numbers printed on shield
CircleBot::CircleBot(uint8_t mtr0num, uint8_t mtr1num, uint8_t mtr2num)
//    : mtrs({mtr0num, mtr1num, mtr2num})

//    : mtrs[0](mtr0num)
//    , mtrs[1](mtr1num)
//    , mtrs[2](mtr2num)
{
    mtrs = new(spaceformtrs) AF_DCMotor(mtr0num);
    new(spaceformtrs+1) AF_DCMotor(mtr1num);
    new(spaceformtrs+2) AF_DCMotor(mtr2num);
}

// Moves the robot given values from -1000 to 1000 in x, y, and z
//   and 0 to 62,832 in gyro
// x: lateral movement, y: forward movement, z: rotational movement
//   gyro: 
void CircleBot::Move(int x, int y, int z, unsigned int gyro) {

    if (gyro) {
        float fgyro = gyro/10000.0;
        float cosgyro = cos(fgyro);
        float singyro = sin(fgyro);

        x = (int)(x * cosgyro - y * singyro); // rotating vectors
        y = (int)(x * singyro + y * cosgyro); // with respect to gyro angle
    }

    int speed2 = ((long)sinTwoForty * (long)y + (long)cosTwoForty * (long)x)/1000 + z;
    int speed1 = ((long)sinOneTwenty * (long)y + (long)cosOneTwenty * (long)x)/1000 + z;
    int speed0 = x + z;

    int highSpeed = max(max(abs(speed0),abs(speed1)),abs(speed2));

    float speedDivisor = max(1000, highSpeed)/1000.0;

    speed2 /= speedDivisor;
    speed1 /= speedDivisor;
    speed0 /= speedDivisor;

    RunMotor(0,  speed0);
    RunMotor(1, -speed1); // This motor is backward
    RunMotor(2,  speed2);

    debugInfo.rotatedInputs.x = x;
    debugInfo.rotatedInputs.y = y;
    debugInfo.rotatedInputs.z = z;
}

// Moves one motor given values from -1000 to 1000
// motornum is 0-2
void CircleBot::RunMotor(uint8_t motornum, int speed) {

    if      (speed < -1000) speed = -1000;
    else if (speed >  1000) speed =  1000;

    int fspeed;

    if (speed != 0) {
        fspeed = (int)(speed / 5.5555);
        if (speed > 0) fspeed += 75; // threshold for actually moving
        else           fspeed -= 75;
    }
    else {
        fspeed = 0;
    }
    RunRawMotor(motornum, fspeed);
}

// Moves all motors given values from -1000 to 1000
void CircleBot::RunMotors(int speed0, int speed1, int speed2) {
    RunMotor(0, speed0);
    RunMotor(1, speed1);
    RunMotor(2, speed2);
}

// Moves one motor given a value from -255 to 255 without scaling
// motornum is 0-2
void CircleBot::RunRawMotor(uint8_t motornum, int speed) {

    uint8_t drctn = speed < 0? BACKWARD : FORWARD;

    mtrs[motornum].setSpeed(abs(speed)); //set the speed
    mtrs[motornum].run(drctn); //run the motor

    // Set the debug info to match real life
    //debugInfo.mtrspeeds[motornum] = fspeed;
    debugInfo.mtrspeeds[motornum] = speed;

}
    

// Moves all motors given values from -255 to 255 without scaling
void CircleBot::RunRawMotors(int speed0, int speed1, int speed2) {
    RunRawMotor(0, speed0);
    RunRawMotor(1, speed1);
    RunRawMotor(2, speed2);
}

