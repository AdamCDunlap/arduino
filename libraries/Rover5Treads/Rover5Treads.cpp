#include <Rover5Treads.h>
#include <Arduino.h>

Rover5Treads::Rover5Treads(uint8_t frontleftnum, uint8_t frontrightnum,
                           uint8_t backleftnum , uint8_t backrightnum )
    : frontleft (frontleftnum)
    , frontright(frontrightnum)
    , backleft  (backleftnum)
    , backright (backrightnum)
{

}

void Rover5Treads::arcadeDrive(int fwd, int rotate) {
    if (fwd    >= minInVal && fwd    <= maxInVal &&
        rotate >= minInVal && rotate <= maxInVal) {
        int leftvel =  fwd + rotate;
        int rightvel = fwd - rotate;
        Serial.print("Left:  "); Serial.print(leftvel);  Serial.print('\t');
        Serial.print("Right: "); Serial.print(rightvel); Serial.print('\t');
        Serial.println();
        int maximum = max(abs(leftvel), abs(rightvel));
        Serial.print("Maximum: "); Serial.println(maximum);
        if (maximum > maxInVal) {
            float divisor = (float)maximum/(float)maxInVal;
            leftvel  /= divisor;
            rightvel /= divisor;
        }
        leftspd = abs(leftvel);
        leftdrctn = leftvel > 0? FORWARD : BACKWARD;
        rightspd = abs(rightvel);
        rightdrctn = rightvel > 0? FORWARD : BACKWARD;

        idle();
    }
}

void Rover5Treads::tankDrive(int left, int right) {
    leftspd = abs(left);
    leftdrctn = left > 0? FORWARD : BACKWARD;
    rightspd = abs(right);
    rightdrctn = right > 0? FORWARD : BACKWARD;

    idle();
}

void Rover5Treads::idle() {
     frontleft.setSpeed(leftspd);
      backleft.setSpeed(leftspd);
     frontleft.run(leftdrctn);
      backleft.run(leftdrctn);

    frontright.setSpeed(rightspd);
     backright.setSpeed(rightspd);
    frontright.run(rightdrctn);
     backright.run(rightdrctn);
    Serial.print("Leftraw:  "); Serial.print(leftspd);  Serial.print('\t');
    Serial.print("Rightraw: "); Serial.print(rightspd); Serial.print('\t');
    Serial.println();
}
