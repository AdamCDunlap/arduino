#include <Wire.h>
#include <Servo.h>

void sendNumber(int num);
void moveMotors(int mtr1, int mtr2, int mtr3, int mtr4);
template<class T> void normalize4(T* a, T* b, T* c, T* d, T highest);
#define DEBUG_VAR(x) do {Serial.print(#x ": "); Serial.print(x); Serial.print('\t'); } while (0)

void setup() {
    Serial.begin(115200);
    Wire.begin();

}

void loop() {
    long frontRight, frontLeft, backRight, backLeft;

    int xraw = analogRead(0);
    int yraw = analogRead(1);

    int xval = map(xraw, 0, 1023, -255, +255);
    int yval = map(yraw, 0, 1023, -255, +255);

    frontRight = +yval - xval;
    frontLeft  = +yval + xval;
     backRight = +yval + xval;
     backLeft  = +yval - xval;

    normalize4(&frontLeft, &backLeft, &frontRight, &backRight, 255L);

    moveMotors(-backRight, frontRight, -backLeft, frontLeft);

    static unsigned long lastSerialTime = 0;
    if (millis() - lastSerialTime > 200) {
        char buf[128];
        sprintf(buf, "xval: %4d yval: %4d frontRight: %4ld frontLeft: %4ld backRight: %4ld backLeft: %4ld",
                      xval,     yval,     frontRight,      frontLeft,      backRight,      backLeft       );
        Serial.println(buf);
        lastSerialTime = millis();
    }
}


void sendNumber(int num) {
    uint8_t* breakdown = reinterpret_cast<uint8_t *>(&num);
    Wire.write(breakdown, 2);
    //Serial.println(*(breakdown+0), BIN);
    //Serial.println(*(breakdown+1), BIN);
}


void moveMotors(int mtr1, int mtr2, int mtr3, int mtr4) {
    Wire.beginTransmission(12);
    sendNumber((abs(mtr1)<50)? 0 : mtr1);
    sendNumber((abs(mtr2)<50)? 0 : mtr2);
    sendNumber((abs(mtr3)<50)? 0 : mtr3);
    sendNumber((abs(mtr4)<50)? 0 : mtr4);
    Wire.endTransmission();
}

#ifndef max
#define max(a, b) ((a)>(b)?(a):(b))
#endif
template<class T> void normalize4(T* a, T* b, T* c, T* d, T maximum) {
    T A = abs(*a);
    T B = abs(*b);
    T C = abs(*c);
    T D = abs(*d);
    T highest  = max(A, B);
    T highest1 = max(C, D);
       highest = max(highest, highest1);
     
    // If all are below the max, we don't need to do anything
    if (highest <= maximum) return;
    
    // The highest/2 factor is to round correctly
    T roundval = highest/2;
    *a = ((*a * maximum) + roundval)/highest;
    *b = ((*b * maximum) + roundval)/highest;
    *c = ((*c * maximum) + roundval)/highest;
    *d = ((*d * maximum) + roundval)/highest;
}
