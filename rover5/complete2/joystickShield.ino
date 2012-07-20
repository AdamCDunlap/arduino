/*
***Hardware setup***

arduino 1 -- motor controller interface | arduino 2 -- logic:
  00 - serial                           |   00 - serial
  01 - serial                           |   01 - serial
  02 - encoder1A                        |   02 - 
  03 - encoder1B                        |   03 - 
  04 - encoder2A                        |   04 - ultrasonic
  05 - mtr1pwm                          |   05 - 
  06 - mtr1dir                          |   06 - 
  07 - encoder2B                        |   07 - ultrasonic
  08 - encoder3A                        |   08 - 
  09 - mtr2pwm                          |   09 - turret servo
  10 - mtr3pwm                          |   10 - 
  11 - mtr4pwm                          |   11 - 
  12 - encoder3B                        |   12 - 
  13 - mtr2dir                          |   13 - 
a 14 - encoder4A                        | a 14 - mtr1current
n 15 - encoder4B                        | n 15 - mtr2current
a 16 - mtr3dir                          | a 16 - mtr3current
l 17 - mtr4dir                          | l 17 - mtr4current
o 18 - i2c communication                | o 18 - i2c
g 19 - i2c                              | g 19 - i2c
*/

#include <Wire.h>
#include <Servo.h>

void sendNumber(int num);
void moveMotors(int mtr1, int mtr2, int mtr3, int mtr4);
//template<class T> void normalize4(T* a, T* b, T* c, T* d);

void setup() {
    Serial.begin(115200);
    Wire.begin();

}

void loop() {
    int frontRight, frontLeft, backRight, backLeft;

    int xval = map(analogRead(0), 0, 1023, -255, 255);
    int yval = map(analogRead(1), 0, 1023, -255, 255);

    //frontRight = +yval - xval;
    //frontLeft  = +yval + xval;
    // backRight = +yval + xval;
    // backLeft  = +yval - xval;

    //normalize4(&frontLeft, &backLeft, &frontRight, &backRight);


    //static int i=0;
    //bool goingup = true;
    frontRight = frontLeft = backRight = backLeft = 64;
    //i += goingup? 1 : -1;
    //if (i >=  255) goingup = false;
    //if (i <= -255) goingup = true;

    moveMotors(frontLeft, backLeft, frontRight, backRight);
    //Serial.println(i);
    delay(100);
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

//template<class T>
//void normalize4(T* a, T* b, T* c, T* d, T* highest) {
//    T* maxval = (*a)>(*b)? a : b;
//    maxval = (*maxval)>(*c)? maxval : c;
//    maxval = (*maxval)>(*d)? maxval : d;
//
//    if (*maxval <= *highest) return;
//
//}
//    
//void normaize4(int* a, int* b, int* c, int* d, int highest) {
//    int maxval =      (*a)>(*b)?     *a : *b;
//        maxval = (*maxval)>(*c)? maxval : *c;
//        maxval = (*maxval)>(*d)? maxval : *d;
//
//    if (maxval <= highest) return;
//}
