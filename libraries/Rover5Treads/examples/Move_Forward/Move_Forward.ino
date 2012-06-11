#include <Rover5Treads.h>
#include <AFMotor.h>

volatile int dummy;

Rover5Treads rover;

void setup() {
    Serial.begin(115200);
    
    rover.move(255, 0);
}

void loop() {
    delay(1000);
}
