// SoftBlinkTest.ino
#include <SoftBlink.h>

// (pin number, fade time)
SoftBlink blueLED(9, 500);

void setup() {
    Serial.begin(115200);
    digitalWrite(12, HIGH);
}

void loop() {
    blueLED.write(digitalRead(12));
}
