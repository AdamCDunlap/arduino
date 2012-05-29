#ifndef CircleBot_h
#define CircleBot_h

#include <AFMotor.h>

class CircleBot {
public:
    // Constructor
    // Use numbers printed on shield
    CircleBot(uint8_t mtr0num=1, uint8_t mtr1num=3, uint8_t mtr2num=4);

    // Moves the robot given values from -1000 to 1000 in x, y, and z
    //   and 0 to 62,832 in gyro
    // x: lateral movement, y: forward movement, z: rotational movement
    //   gyro: 
    void Move(int x, int y, int z = 0, unsigned int gyro=0);

    // Moves one motor given a value from -1000 to 1000
    // motornum is 0-2
    void RunMotor(uint8_t motornum, int speed);

    // Moves all motors given values from -1000 to 1000
    void RunMotors(int speed0, int speed1, int speed2);

    // Moves one motor given a value from -255 to 255 without scaling
    // motornum is 0-2
    void RunRawMotor(uint8_t motornum, int speed);

    // Moves all motors given values from -255 to 255 without scaling
    void RunRawMotors(int speed0, int speed1, int speed2);

    struct DebuggingInfo {
        int mtrspeeds[3];
        struct RotatedInputs {
            int x;
            int y;
            int z;
        } rotatedInputs;
    } debugInfo;
private:
    static const uint8_t numMtrs = 3;

    // create space for the motors beforehand and use placement new
    AF_DCMotor* mtrs;
    char spaceformtrs[sizeof(AF_DCMotor) * numMtrs];

    //AF_DCMotor mtrs[numMtrs];
//    AF_DCMotor mtrs0;
//    AF_DCMotor mtrs1;
//    AF_DCMotor mtrs2;

    static const int cosOneTwenty = -500;
    static const int sinOneTwenty = +866;
    static const int sinTwoForty  = -866;
    static const int cosTwoForty  = -500;
};

#endif
