#include <Servo.h>
#include <RunningMedian.h>
// Get the line numbering to behave
byte dummy;
#line 6

// Inputs
const byte startPin = 12;
const byte leftDistPin = A5;
const byte rightDistPin = A4;
const byte leftFeelerPin = 6;

// Outputs
const byte jagPin = 9;
const byte steerPin = 8;
const byte rightledpow = A0;
const byte rightledgnd = A1;
const byte leftledpow = 3;
const byte leftledgnd = 4;

Servo jag;
Servo steer;
const byte leftDistThresh = 40;
const byte rightDistThresh = 40;

// Returns distance in cm
byte readIR(const byte pin) {
    return 1/(0.0000868056*analogRead(pin)-0.00222222);
}

namespace st { // States for state machine
    enum robotstate_t {
        beginning,
        beforeButton,
        afterButtonWait,
        firstStraightaway,
        curve,
        secondStraightaway,
    };
    enum statepos_t {
        begin,
        idle,
        end,
    };
    enum substate_t {
        noSubstate,
        backing,
    };
}
    
          
void setup() {
    Serial.begin(115200);

    pinMode(rightledpow, OUTPUT); pinMode(rightledgnd, OUTPUT);
    pinMode( leftledpow, OUTPUT); pinMode( leftledgnd, OUTPUT);
    digitalWrite(rightledgnd, LOW); digitalWrite(leftledgnd, LOW);
    jag.attach(jagPin);
    steer.attach(steerPin);
    digitalWrite(leftFeelerPin, HIGH);
    digitalWrite(startPin, HIGH);
}

/* 1. Get sensor values
 * 2. Police state machine
 * 3. Run state machine
 * 4. Run outputs
 */
void loop() {
// 1.Get sensor values
    static RunningMedian leftMedian;
    static RunningMedian rightMedian;
    leftMedian.add(readIR(leftDistPin));
    rightMedian.add(readIR(rightDistPin));

    unsigned int leftDist = leftMedian.getMedian();
    unsigned int rightDist = rightMedian.getMedian();
    bool leftFeeler = !digitalRead(leftFeelerPin);
    bool startButton = !digitalRead(startPin);
    // Display debugging leds
    digitalWrite(leftledpow, leftDist < 40);
    digitalWrite(rightledpow, rightDist < 40);

    Serial.print(leftDist);
    Serial.print('\t');
    Serial.print(rightDist);
    Serial.print('\t');
// 2.Police state machine
    int8_t jagspeed = 0; // -100 to 100
    int8_t turnamt = 0; // -100 to 100

    static st::robotstate_t robotstate = st::beginning;
    static st::robotstate_t nextrobotstate = st::beforeButton;
    st::statepos_t statepos = st::begin;
    static st::substate_t substate = st::noSubstate;

    if (statepos == st::begin) statepos = st::idle;

    static unsigned long stateStartTime = millis();
    if (robotstate != nextrobotstate) {
        if (false/*statepos = st::idle*/) statepos = st::end;
        else {
            statepos = st::begin;
            robotstate = nextrobotstate;
            stateStartTime = millis();
        }
    }
    unsigned long timeIntoState = millis() - stateStartTime;
    unsigned long timePlaceholder = 0;

    switch(robotstate) {
    case st::beforeButton:
        if(startButton) nextrobotstate = st::afterButtonWait;
        break;
    case st::afterButtonWait:
        if(timeIntoState > 1000) nextrobotstate = st::firstStraightaway;
        break;
    case st::firstStraightaway: case st::curve: case st::secondStraightaway:
        //if(timeIntoState > 3000 || left)
        //    nextrobotstate = st::curve;
        if (substate == st::backing) {
            if (millis() - timePlaceholder > 1500) {
                substate = st::noSubstate;
            }
            jagspeed = -10;
            turnamt = 30;
        }
        else if (leftDist < leftDistThresh && rightDist >= rightDistThresh) {
            jagspeed = 10;
            turnamt = 70;
        }
        else if (leftDist >= leftDistThresh &&  rightDist < rightDistThresh) {
            jagspeed = 10;
            turnamt = -70;
        }
        else if (leftDist <= 27 && rightDist <= 27) {
            jagspeed = -10;
            turnamt = 30;
            substate = st::backing;
            timePlaceholder = millis();
        }
        else {
            jagspeed = 15;
            turnamt = 0;
        }
        break;
    default:
        Serial.print("Undefined state: "); Serial.print(robotstate); Serial.print('\t');
    }


// Jag datasheet says it wants a puse between .67ms and 2.33ms
    jag.writeMicroseconds(map(jagspeed, -100, 100, 670, 2330));
// May have to change the 0,180 to match what the servo wants
    steer.write(map(turnamt, -100, 100, 0, 180));

// End the serial line after printing everything
    Serial.println();
}
