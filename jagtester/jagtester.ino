// jagtester.ino
#include <Servo.h>

Servo jag;

void setup() {
    jag.attach(9);
}

void loop() {
    //// Forward and backward
    jag.writeMicroseconds(map(analogRead(A0), 0, 1023, 670, 2330));
    // Only go forward
    //jag.writeMicroseconds(map(analogRead(A0), 0, 1023, 1500, 2330));
    // Only go backward
    //jag.writeMicroseconds(map(analogRead(A0), 0, 1023,  670, 1500));
}
