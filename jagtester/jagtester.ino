// jagtester.ino
#include <Servo.h>

Servo jag;

void setup() {
    jag.attach(9);
}

void loop() {
    jag.write(map(analogRead(A0), 0, 1023, 0, 180));
}
