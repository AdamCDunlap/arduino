#include <Rover5Treads.h>
#include <AFMotor.h>

volatile int dummy;

Rover5Treads rover;

void setup() {
    Serial.begin(115200);
    
    rover.move(255, 0);
    delay(3000);
    rover.move(0, 255);
    delay(3000);
    rover.move(-255, 0);
    delay(3000);
    rover.move(100, 200);
    delay(2000);
    rover.stop();
}

void loop() {
    rover.idle();
    delay(1000);
}
