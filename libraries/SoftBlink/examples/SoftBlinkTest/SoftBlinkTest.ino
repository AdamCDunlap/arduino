// SoftBlinkTest.ino
#include <SoftBlink.h>

// (pin number, fade time)
SoftBlink blueLED(9, 500);

void setup() {

    // Set the pull-up resistor for a switch on pin 12
    // We have to do this or the reading will give random values
    digitalWrite(12, HIGH);
}

void loop() {
    bool switchVal = digitalRead(12);
    blueLED.write(switchVal);
}
