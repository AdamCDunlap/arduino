
/* Things to consider:
 *  During Sleep:
 *   Extended Standby Mode
 *   Turn off:
 *    ADC
 *    Analog Comparator
 *    Brown-out Detector
 *    Internal Voltage Reference
 *    WDT
 *    On-Chip debug system?
 *    
 */

//#define DEBUG
#define LEDARRAY



char dummy; // Signal to arduino preprocessor

#ifdef ARDUINO
#include <Time.h>

#ifdef LEDARRAY
#include <LEDArray.h>
#endif // LEDARRAY

#else // defined ARDUINO
#include <stdio.h>
#include <ctime>
using namespace std;

#undef LEDARRAY
#endif //defined ARDUINO


#ifdef DEBUG

#ifdef ARDUINO
#define DEBUG_VAR(variable) do {Serial.print(#variable " = "); Serial.print(variable); Serial.print("\t");} while (0)
#define DEBUG_LAST() do { Serial.println(); } while (0)

#else // defined ARDUINO
#define DEBUG_VAR(variable) do { printf( #variable " = %d\t", variable) } while (o)
#define DEBUG_LAST() do { cout << endl; } while (0)
#endif // defined ARDUINO

#else // DEBUG
#define DEBUG_VAR(x) (void) 0
#define DEBUG_LAST() (void) 0
#endif //DEBUG

// Prototypes
void processTimeSyncMessage();
void printTime();
void setup();
void loop();

#ifndef ARDUINO
int main() {
    setup();
    loop();
    return 0;
}
#endif //not defined ARDUINO

namespace leds {
    const int it_is = 0;
    //                         0, 5, 10, 15, 20, 25, 30
    const int minuteNums[] = {21, 5,  1,  2,  3, -1, 4 };
    // 25 is nonexistant, special things will be done later;  0 is "o'clock"


    const int minutes = 6; //unnecessary
    const int to = 7;
    const int past = 8;
    
    //                      1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12
    const int hourNums[] = {9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
}

//struct Bits
//{
//    unsigned b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
//};
//union CBits
//{
//    Bits bits;
//    uint8_t byte;
//};

// Array that holds the state of each led
bool states[22];

#ifdef LEDARRAY
LEDArray disp(2,3,4);
#endif //LEDARRAY

void setup() {
#ifdef ARDUINO
    Serial.begin(9600);
#endif

#ifdef LEDARRAY
    disp.setPrintDuration(500);
#endif
    //set_sleep_mode(SLEEP_MODE_EXT_STANDBY);

//    pinMode(leds::it_is, OUTPUT);
//    for (short i=0;i<7;i++)
//        pinMode(leds::minuteNums[i], OUTPUT);
//    pinMode(leds::minutes, OUTPUT);
//    pinMode(leds::to, OUTPUT);
//    pinMode(leds::past, OUTPUT);
//    for (short i = 0; i < 12; i++)
//        pinMode(leds::hourNums[i], OUTPUT);

//    digitalWrite(leds::it_is, HIGH);
}


void loop() {
//    digitalWrite(leds::it_is, LOW);
//    for (short i=0;i<7;i++)
//        digitalWrite(leds::minuteNums[i], LOW);
//    digitalWrite(leds::minutes, LOW);
//    digitalWrite(leds::to, LOW);
//    digitalWrite(leds::past, LOW);
//    for (short i = 0; i < 12; i++)
//        digitalWrite(leds::hourNums[i], LOW);


    //delay(1000);
    //adjustTime(120); // Speed up time 2 minutes per loop

    #ifdef ARDUINO
    if (Serial.available()) processTimeSyncMessage();
    #endif // ARDUINO


    short hour;
    short curMinute;
    #ifdef ARDUINO
    time_t rawTime = now();
    hour = hourFormat12(rawTime);
    curMinute = minute(rawTime);

    #else //ARDUINO
    time_t rawTime = time(NULL);
    struct tm* timeInfo = localtime( &rawTime);

    hour = timeInfo->tm_hour;
    if (hour > 12) hour -= 12;
    curMinute = timeInfo->tm_min;
    #endif

    short nextHour = hour+1;
    if (nextHour == 13) nextHour = 1;

    short fiveMinute = (curMinute + 2) / 5; // Divide, rounding correctly
    //if (fiveMinute == 12) fiveMinute = 0;
    static short lastFiveMinute = -1; // impossible value to ensure printing

    // the 12 -> 0 transition doesn't change the output
    bool changed = (lastFiveMinute != fiveMinute) 
        && !(lastFiveMinute == 12 && fiveMinute == 0);

    if (changed) {

        for (unsigned short i = 0; i < sizeof(states) / sizeof(states[0]); i++) {
            states[i] = false;
        }

        //digitalWrite(leds::it_is, HIGH);
        states[leds::it_is] = true;

        short sectionsToNearestHour;

        if (fiveMinute <= 6) {
            //digitalWrite(leds::hourNums[hour-1], HIGH);
            states[leds::hourNums[hour-1]] = true;
            if (fiveMinute != 0) {
                //digitalWrite(leds::past, HIGH);
                states[leds::past] = true;
            }
            
            sectionsToNearestHour = fiveMinute;
        }
        else {
            //digitalWrite(leds::h urNums[nextH ur], HIGH);
            states[leds::hourNums[nextHour-1]] = true;
            if (fiveMinute != 12) {
                //digitalWrite(leds::to, HIGH);
                states[leds::to] = true;
            }
            sectionsToNearestHour = 12 - fiveMinute;
        }

        if (sectionsToNearestHour == 5) { // We have to do 2 leds for 25

            // Set 20 and 5 on
            //digitalWrite(leds::minuteNums[1], HIGH);
            states[leds::minuteNums[4]] = true;
            //digitalWrite(leds::minuteNums[1], HIGH);
            states[leds::minuteNums[1]] = true;
        }
        else {
            //digitalWrite(leds::minuteNums[fiveMinute], HIGH);
            states[leds::minuteNums[sectionsToNearestHour]] = true;
        }
        
        DEBUG_VAR(hour);
        DEBUG_VAR(curMinute);
        DEBUG_VAR(fiveMinute);
        DEBUG_LAST();

        lastFiveMinute = fiveMinute;

// If an LEDArray is connected, print the time every time through the loop
#ifdef LEDARRAY
}
#endif
        printTime();
        
#ifndef LEDARRAY
    }
#endif
}

#ifdef ARDUINO
void processTimeSyncMessage() {

    // The character used to start the time-setting
    char timeHeader = 'T';

  // if time sync available from serial port, update time and return true
    while(Serial.available() >=  7 ){  // time message consists of a header and six ascii digits
        char c = Serial.read();
        if( c == timeHeader ) {
            int timeBuf[3] = {0,0,0};
            for(int j=0;j<3;j++) {
                for(int i=0; i < 2; i++){
                    c = Serial.read();
                    if( c >= '0' && c <= '9'){
                        timeBuf[j] = (10 * timeBuf[j]) + (c - '0') ; // convert digits to a number    
                    }
                }
            }
            Serial.println(); 
            Serial.print(timeBuf[0]); Serial.print(':');
            Serial.print(timeBuf[1]); Serial.print('.');
            Serial.print(timeBuf[2]);
            Serial.println();
            // Sync arduino clock to the time received on the serial port
            setTime(timeBuf[0], timeBuf[1], timeBuf[2], 0, 0, 1971);   
            }
    }
}
#endif // ARDUINO

void printTime() {

#ifdef ARDUINO

#ifdef LEDARRAY
#define PRINT(...) do {disp.print(__VA_ARGS__); Serial.print(__VA_ARGS);} while (0)

#else //LEDARRAY
#define PRINT(...) Serial.print(__VA_ARGS__)
#endif //LEDARRAY

#else // Defined ARDUINO
#define PRINT(...) printf(__VA_ARGS__)
#endif //Defined ARDUINO


    if(states[leds::it_is])         PRINT("It is ");
    if(states[leds::minuteNums[2]]) PRINT("ten ");
    if(states[leds::minuteNums[3]]) PRINT("quarter ");
    if(states[leds::minuteNums[4]]) PRINT("twenty ");
    if(states[leds::minuteNums[6]]) PRINT("half ");
    if(states[leds::minuteNums[1]]) PRINT("five ");
    if(states[leds::minutes])       PRINT("minutes ");
    if(states[leds::to])            PRINT("to ");
    if(states[leds::past])          PRINT("past ");
    if(states[leds::hourNums[0]])   PRINT("one ");
    if(states[leds::hourNums[1]])   PRINT("two ");
    if(states[leds::hourNums[2]])   PRINT("three ");
    if(states[leds::hourNums[3]])   PRINT("four ");
    if(states[leds::hourNums[4]])   PRINT("five ");
    if(states[leds::hourNums[5]])   PRINT("six ");
    if(states[leds::hourNums[6]])   PRINT("seven ");
    if(states[leds::hourNums[7]])   PRINT("eight ");
    if(states[leds::hourNums[8]])   PRINT("nine ");
    if(states[leds::hourNums[9]])   PRINT("ten ");
    if(states[leds::hourNums[10]])  PRINT("eleven ");
    if(states[leds::hourNums[11]])  PRINT("twelve ");
    if(states[leds::minuteNums[0]]) PRINT("o'clock ");

    PRINT("\n");
}
