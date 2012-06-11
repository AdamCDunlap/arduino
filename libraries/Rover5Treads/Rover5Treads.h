#ifndef ROVER5TREADS_H
#define ROVER5TREADS_H
#include <AFMotor.h>

class Rover5Treads {
public:
    Rover5Treads(uint8_t frontleftnum=1, uint8_t frontrightnum=2,
                 uint8_t backleftnum =4, uint8_t backrightnum =3);

    void arcadeDrive(int fwd, int rotate);
    void tankDrive(int left, int right);

    void move(int fwd, int rotate) { arcadeDrive(fwd, rotate); }
    void stop() { tankDrive(stopVal, stopVal); }

    void idle();

    static const int minInVal = -255;
    static const int maxInVal =  255;
    static const int stopVal = 0;
private:
    uint8_t leftspd;
    uint8_t rightspd;
    uint8_t leftdrctn;
    uint8_t rightdrctn;

    AF_DCMotor frontleft;
    AF_DCMotor frontright;
    AF_DCMotor backleft;
    AF_DCMotor backright;
};

#endif // ROVER5TREADS_H
