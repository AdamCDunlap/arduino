#include <Wire.h>
#include <Servo.h>

void sendNumber(int num);
void moveMotors(int mtr1, int mtr2, int mtr3, int mtr4);

void setup() {
    Serial.begin(115200);
    Wire.begin();

}

void loop() {
    static bool goingup = true;
    static int i = 0;
    int frontRight, frontLeft, backRight, backLeft;
    i += goingup? 1 : -1;
    if (i >=  255) goingup = false;
    if (i <= -255) goingup = true;

    //frontRight = frontLeft = backRight = backLeft = i;
    frontRight =  -177;
     frontLeft =   177;
     backRight =   177;
      backLeft =  -177;
    moveMotors(-backRight, frontRight, -backLeft, frontLeft);
    //Serial.println(i);
    delay(50);
}


void sendNumber(int num) {
    uint8_t* breakdown = reinterpret_cast<uint8_t *>(&num);
    Wire.write(breakdown, 2);
    //Serial.println(*(breakdown+0), BIN);
    //Serial.println(*(breakdown+1), BIN);
}


void moveMotors(int mtr1, int mtr2, int mtr3, int mtr4) {
    Wire.beginTransmission(12);
    sendNumber(mtr1);
    sendNumber(mtr2);
    sendNumber(mtr3);
    sendNumber(mtr4);
    Wire.endTransmission();
}
