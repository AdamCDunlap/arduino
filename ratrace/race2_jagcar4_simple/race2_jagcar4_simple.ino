#include <Servo.h>
#include <RunningMedian.h>
// Get the line numbering to behave
byte dummy;
#line 6
//#define PROPROTIONAL

// Inputs
const byte startPin = 12;
const byte leftDistPin = A5;
const byte rightDistPin = A4;
const byte leftFeelerPin = 3;
const byte batPin = A0;

// Outputs
const byte jagPin = 9;
const byte steerPin = 8;
const byte rightledpow = A2;
const byte rightledgnd = A3;
const byte leftledpow = 7;
const byte leftledgnd = 4;
const byte pwrledpow = 6;
const byte pwrledgnd = 5;

Servo jag;
Servo steer;
const unsigned int leftDistThresh = 150;
const unsigned int rightDistThresh = 150;

// Returns distance in cm
byte readIR(const byte pin) {
    return 1/(0.0000868056*(float)analogRead(pin)-0.00222222);
}

namespace st { // States for state machine
    enum robotstate_t {
        beginning,
        beforeButton,
        afterButtonWait,
        redneck,
        findDirection,
        straightaway,
        endofstraightaway,
        recoverFromWall,
        uturn,
        farstraightaway,
        farS,
        midS,
        closeS,
        ccwFinish,
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
    enum direction_t {
        dontknow,
        clockwise,
        counterclockwise,
    };
}


void avoidWalls(unsigned int leftDist, unsigned int rightDist,
                int8_t* jagspeed, int8_t* turnamt,
                int8_t nominalspeed = 20, int8_t nominalsteer = 0) {
    if (leftDist > leftDistThresh) {
        *turnamt = nominalsteer+40;
        *jagspeed = 0.8*nominalspeed;
    }
    else if (rightDist > rightDistThresh) {
        *turnamt = nominalsteer-40;
        *jagspeed = 0.8*nominalspeed;
    }
    else {
        *turnamt = nominalsteer;
        *jagspeed = nominalspeed;
    }
}
#define AVOID_WALLS(nominalspeed, nominalsteer)\
    avoidWalls(leftDist, rightDist, &jagspeed, &turnamt,\
               nominalspeed, nominalsteer)
    
          
void setup() {
    Serial.begin(115200);

    pinMode(rightledpow, OUTPUT); pinMode(rightledgnd, OUTPUT);
    pinMode( leftledpow, OUTPUT); pinMode( leftledgnd, OUTPUT);
    pinMode(  pwrledpow, OUTPUT); pinMode(  pwrledgnd, OUTPUT);
    digitalWrite(rightledgnd, LOW); digitalWrite(leftledgnd, LOW);
    digitalWrite(pwrledgnd, LOW);
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

    unsigned int leftDist = leftMedian.getMedian();//readIR(leftDistPin);//leftMedian.getMedian();
    unsigned int rightDist = rightMedian.getMedian(); //readIR(rightDistPin); //rightMedian.getMedian();
    unsigned int powerlvl = analogRead(batPin); // powerlvl is voltage*512
    bool leftFeeler = !digitalRead(leftFeelerPin);
    bool startButton = !digitalRead(startPin);
    // Display debugging leds
    digitalWrite(leftledpow, leftDist > leftDistThresh);
    digitalWrite(rightledpow, rightDist > leftDistThresh);
//    digitalWrite(pwrledpow, powerlvl < 3700);
    
    static unsigned long lastPrintTime = 0;
    if (millis() - lastPrintTime > 100) {
        lastPrintTime = millis();
        Serial.print(leftDist);
        Serial.print('\t');
        Serial.print(rightDist);
        Serial.print('\t');

        Serial.println();
    }

    int8_t turnamt = 0;
    int8_t jagspeed = 0;
    
    turnamt = (leftDist - rightDist);


// Jag datasheet says it wants a puse between .67ms and 2.33ms, so go a little inside that
    jag.writeMicroseconds(map(jagspeed, -100, 100, 720, 2280));
// May have to change the 0,180 to match what the servo wants
    steer.write(map(turnamt, -100, 100, 0, 150));

}





//        if (substate == st::backing) {
//            if (millis() - timePlaceholder > 1500) {
//                substate = st::noSubstate;
//            }
//            if (direction == st::clockwise) {
//                turnamt = 30;
//            }
//            else if (direction == st::counterclockwise) {
//                turnamt = -30;
//            }
//            else {
//                turnamt = 30;
//            }
//            jagspeed = -13;
//        }
//        else if (leftDist > leftDistThresh && rightDist <= rightDistThresh) {
//            jagspeed = 24;
//            turnamt = 40;
//        }
//        else if (leftDist <= leftDistThresh &&  rightDist > rightDistThresh) {
//            jagspeed = 24;
//            turnamt = -40;
//        }
//        else if (leftDist >= leftDistThresh && rightDist >= rightDistThresh) {
//            // Both sensors see a wall
//            jagspeed = -13;
//            turnamt = 0;
//            substate = st::backing;
//            timePlaceholder = millis();
//        }
