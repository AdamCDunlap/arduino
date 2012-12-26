// master.ino
#include <Wire.h>

void setup() {
    pinMode(13, OUTPUT);
    Wire.begin();
    do {
        Wire.requestFrom(12, 1);
    } while (Wire.available() < 1);
}

void loop() {
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
}
