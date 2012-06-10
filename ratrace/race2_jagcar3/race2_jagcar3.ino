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
    //return 1/(0.0000868056*(float)analogRead(pin)-0.00222222);
    return map(analogRead(pin), 410, 103, 30, 150);
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
        other,
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


void avoidWalls(unsigned int leftDist, unsigned int rightDist, int diff, 
                int8_t* jagspeed, int8_t* turnamt,
                int8_t nominalspeed = 20, int8_t nominalsteer = 0,
                int8_t steeramt = 40) {
//    if (leftDist > leftDistThresh) {
//        *turnamt = nominalsteer+steeramt;
//        *jagspeed = 0.8*nominalspeed;
//    }
//    else if (rightDist > rightDistThresh) {
//        *turnamt = nominalsteer-steeramt;
//        *jagspeed = 0.8*nominalspeed;
//    }
//    else {
//        *turnamt = nominalsteer;
//        *jagspeed = nominalspeed;
//    }
    *jagspeed = nominalspeed;
    *turnamt = (float)map(diff, -100, 100, -80, 80);
}
#define AVOID_WALLS(nominalspeed, nominalsteer, steeramt)\
    avoidWalls(leftDist, rightDist, diff, &jagspeed, &turnamt,\
               nominalspeed, nominalsteer, steeramt)
    
          
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
    int diff = (int)rightDist - (int)leftDist;
    unsigned int powerlvl = analogRead(batPin); // powerlvl is voltage*512
    bool leftFeeler = !digitalRead(leftFeelerPin);
    bool startButton = !digitalRead(startPin);
    // Display debugging leds
    digitalWrite(leftledpow, leftDist > leftDistThresh);
    digitalWrite(rightledpow, rightDist > leftDistThresh);
//    digitalWrite(pwrledpow, powerlvl < 3700);
    
    static unsigned long lastPrintTime = 0;
    if (millis() - lastPrintTime > 500) {
        lastPrintTime = millis();
//        Serial.print(leftDist);
//        Serial.print('\t');
//        Serial.print(rightDist);
//        Serial.print('\t');
//        Serial.print(diff);
//        Serial.print('\t');
//
//        Serial.println();
    }
    
// 2.Police state machine
    int8_t jagspeed = 0; // -100 to 100
    int8_t turnamt = 0; // -100 to 100

    static st::robotstate_t robotstate = st::beginning;
    static st::robotstate_t nextrobotstate = st::beforeButton;
    st::statepos_t statepos = st::begin;
    static st::substate_t substate = st::noSubstate;
    static st::direction_t direction = st::dontknow;
    
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
        if(timeIntoState > 1000) nextrobotstate = st::other;
        break;
    case st::other:
        if (leftDist >= 60 && leftDist <= 70 && rightDist >= 80 && rightDist <= 90) {
            // Against wall
            jagspeed = -100;
        }
        else {
            AVOID_WALLS(40, -30, 70);
        }
        break;
//    case st::redneck:
//        if (timeIntoState > 400) nextrobotstate = st::findDirection;
//        jagspeed = 90;
//        turnamt = 0;
//        break;
//    case st::findDirection:
//        //AVOID_WALLS(20, 0);
//        jagspeed = 30;
//        if (leftFeeler) {
//            nextrobotstate = st::recoverFromWall;
//            direction = st::counterclockwise;
//        }
//        else if (timeIntoState > 3000) {
//            nextrobotstate = st::straightaway;
//            direction = st::clockwise;
//        }
//        break;
//    case st::straightaway:
//        if (direction == st::clockwise) {
//            AVOID_WALLS(100, 0);
//            if (timeIntoState > 2000) nextrobotstate = st::uturn;
//        }
//        else {
//            AVOID_WALLS(60, 0);
//            if (timeIntoState > 15000) nextrobotstate = st::beforeButton;
//        }
//        break;
//    case st::endofstraightaway:
//        if (leftFeeler) nextrobotstate = st::recoverFromWall;
//        jagspeed = 25;
//        break;
//    case st::recoverFromWall:
//        if (direction == st::clockwise) {
//            if (timeIntoState > 750) nextrobotstate = st::uturn;
//            jagspeed = -40;
//        }
//        else {
//            if (timeIntoState > 750) nextrobotstate = st::closeS;
//            jagspeed = -40;
//        }
//        break;
//    case st::uturn:
//        if (direction == st::clockwise) {
//            if (timeIntoState > 3000) nextrobotstate = st::farstraightaway;
//            jagspeed = 30;
//            turnamt = 100;
//        }
//        else {
//            if (timeIntoState > 3000) nextrobotstate = st::straightaway;
//            jagspeed = 30;
//            turnamt = -80;
//        }
//        break;
//    case st::farstraightaway:
//        if (direction == st::clockwise) {
//            if(timeIntoState > 500) {
//                nextrobotstate = st::farS;
//            }
//            AVOID_WALLS(30, -60);
//        }
//        else {
//            if (timeIntoState > 500) {
//                nextrobotstate = st::uturn;
//            }
//            AVOID_WALLS(30, 0);
//        }
//        break;
//    case st::farS:
//        if (direction == st::clockwise) {
//                if (timeIntoState > 2000) nextrobotstate = st::midS;
//                AVOID_WALLS(30, -40);
//
//            }
//            else {
//                if (timeIntoState > 3000) nextrobotstate = st::farstraightaway;
//                AVOID_WALLS(30, 35);
//            }
//        break;
//    case st::midS:
//        if (direction == st::clockwise) {
//            if (timeIntoState > 2500) nextrobotstate = st::closeS;
//            AVOID_WALLS(30, 35);
//
//        }
//        else {
//            if (timeIntoState > 3000) nextrobotstate = st::farS;
//            AVOID_WALLS(30, -35);
//        }
//        break;
//    case st::closeS:
//        if (direction == st::clockwise) {
//            if (timeIntoState > 2500) nextrobotstate = st::ccwFinish;
//            AVOID_WALLS(30, -35);
//        }
//        else {
//            if (timeIntoState > 3000) nextrobotstate = st::midS;
//            AVOID_WALLS(30, 35);
//        }
//        break;
//    case st::ccwFinish:
//        if (timeIntoState > 2000) nextrobotstate = st::beforeButton;
//        AVOID_WALLS(0, 0);
//        break;
    default:
        Serial.print("Undefined state: "); Serial.print(robotstate); Serial.print('\t');
    }


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
