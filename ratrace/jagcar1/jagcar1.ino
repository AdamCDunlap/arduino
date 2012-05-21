#include <Servo.h>

const byte switchPin = 12;
const byte jagPin = 9;
const byte steerPin = 8;
const byte leftDistPin = A0;
const byte rightDistPin = A1;

// In inches -- used with map
const byte minSensorDist = 10;
const byte maxSensorDist = 100;

Servo jag;
Servo steer;

void setup() {
    jag.attach(jagPin);
    steer.attach(steerPin);
    while (digitalRead(switchPin)); // Wait until switch is pressed
    delay(1000); // Wait for person to move out of the way
}

// Strategy: go mid-speed when nothing is seen, turn when there's something
// on either left or right, and go backward when theere's something on both 
// sides
void loop() {
    byte leftDist  = map(analogRead(leftDistPin ), 0, 1024,
                        minSensorDist, maxSensorDist);
    byte rightDist = map(analogRead(rightDistPin), 0, 1024,
                        minSensorDist, maxSensorDist);
    int8_t jagspeed = 0; // -100 to 100
    int8_t turnamt = 0; // -100 to 100

    if (leftDist < 10 && rightDist < 10) {
        // running straight into a wall
        jagspeed = -50;
    }
    else if (leftDist >= 30 && rightDist >= 30) {
        // Nothing is there
        jagspeed = 50;
    }
    else if (leftDist >= 30 && rightDist < 30) {
        jagspeed = 20;
        turnamt = 100;
    }
    else if (leftDist < 30 && rightDist >= 30) {
        jagspeed = 20;
        turnamt = 100;
    }
    
// Jag datasheet says it wants a puse between .67ms and 2.33ms
    jag.writeMicroseconds(map(jagspeed, -100, 100, 670, 2330));
// May have to change the 180s to match what the servo wants
    steer.writeMicroseconds(map(turnamt, -100, 100, -180, 180));
}
