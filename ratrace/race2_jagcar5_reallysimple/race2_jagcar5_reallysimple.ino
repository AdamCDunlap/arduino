#include <Servo.h>

const byte switchPin = 12;
const byte jagPin = 9;
const byte steerPin = 8;
const byte leftDistPin = A5;
const byte rightDistPin = A4;

const byte leftFeelerPin = 6;

const byte rightledpow = A0;
const byte rightledgnd = A1;
const byte leftledpow = 3;
const byte leftledgnd = 4;

// In inches -- used with map
//const byte minSensorDist = 10;
//const byte maxSensorDist = 100;

Servo jag;
Servo steer;

void setup() {
    Serial.begin(115200);

    pinMode(rightledpow, OUTPUT);pinMode(rightledgnd, OUTPUT);
    pinMode(leftledpow, OUTPUT); pinMode(leftledgnd, OUTPUT);
    jag.attach(jagPin);
    steer.attach(steerPin);
    digitalWrite(leftFeelerPin, HIGH);
    digitalWrite(switchPin, HIGH);
    while (digitalRead(switchPin)); // Wait until switch is pressed
    delay(1000); // Wait for person to move out of the way
}

// Strategy: go mid-speed when nothing is seen, turn when there's something
// on either left or right, and go backward when theere's something on both 
// sides
void loop() {
    Serial.print(analogRead(A5));
    Serial.print('\t');
    Serial.print(analogRead(A4));
    Serial.println();
    
    //if (!digitalRead(switchPin)) while(true);
    //byte leftDist  = map(analogRead(leftDistPin ), 0, 1024,
    //                    minSensorDist, maxSensorDist);
    unsigned int leftDist = analogRead(leftDistPin);
//    byte rightDist = map(analogRead(rightDistPin), 0, 1024,
//                        minSensorDist, maxSensorDist);
    unsigned int rightDist = analogRead(rightDistPin);
    bool leftFeeler = !digitalRead(leftFeelerPin);
    int8_t jagspeed = 0; // -100 to 100
    int8_t turnamt = 0; // -100 to 100

//    static unsigned long awayFromWallTime;
//    static unsigned long turnedTowardWallTime;
//    bool wasGoingAwayFromWall;
//    if (leftFeeler) {
//        awayFromWallTime = millis() + 1000;
//    }
//    if (millis() < awayFromWallTime) {
//        if (!wasGoingAwayFromWall) {
//            wasGoingAwayFromWall = true;
//            turnedTowardWallTime = millis() + 1500;
//        }
//        jagspeed = 7;
//        turnamt = 50;
//        
//    }
//    else if (millis() < turnedTowardWallTime) {
//        jagspeed = 15;
//        turnamt = -30;
//    }
//    else {
//        jagspeed = 10;
//        turnamt = 0;
//    }

    if (leftDist > 200) {
        digitalWrite(leftledpow, HIGH);
        digitalWrite(leftledgnd, LOW);
    }
    else {
        digitalWrite(leftledpow, LOW);
    }
    if (rightDist > 300) {
        digitalWrite(rightledpow, HIGH);
        digitalWrite(rightledgnd, LOW);
    }
    else {
        digitalWrite(rightledpow, LOW);
    }
    
    
    static unsigned long stopbackuptime = 0;
    if (millis() < stopbackuptime) {
        jagspeed = -10;
        turnamt = 30;
    }
    else if (leftDist > 400 && rightDist > 400) {
        // running straight into a wall
        stopbackuptime = millis() + 1000;
        jagspeed = -20;
        turnamt = 0;
    }
    else if (leftDist <= 200 && rightDist > 300) {
        jagspeed = 20;
        turnamt = -70;
    }
    else if (leftDist > 200 && rightDist <= 300) {
        jagspeed = 20;
        turnamt = 70;
    }
    else {
        // Nothing is there
        jagspeed = 30;
        turnamt = 20;
    }
    
// Jag datasheet says it wants a puse between .67ms and 2.33ms
    jag.writeMicroseconds(map(jagspeed, -100, 100, 670, 2330));
// May have to change the 180s to match what the servo wants
    steer.write(map(turnamt, -100, 100, 0, 180));
    //steer.write(90);
    delay(100);
}
