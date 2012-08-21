#include <Rover5.h>

template <class T> inline void printTimeOf(T nm, unsigned long starttime) {
    unsigned long endtime = micros();
    Serial.print(nm);
    Serial.print(F(": "));
    Serial.print(endtime - starttime);
    Serial.print(' ');
}
#define TIME_OF(nm) for (\
    struct { unsigned long starttime; bool trueonce; }\
        __TIME_OF_STRUCT = {micros(), true};\
    __TIME_OF_STRUCT.trueonce;\
    printTimeOf(nm, __TIME_OF_STRUCT.starttime),\
        __TIME_OF_STRUCT.trueonce = false)

Rover5 bot;

void setup() {

    delay(1000);

    Serial.begin(115200);
    TIME_OF(F("hello")) Serial.println(F("Hello!"));
    bot.begin();
    Serial.println(F("Bot begun"));
}

void loop() {
    TIME_OF(F("Run")) bot.Run(255, 255, 255, 255);

    int speeds[4];
    TIME_OF(F("Get")) bot.GetSpeeds(speeds);
    
    TIME_OF(F("prnt")) {
        for (uint8_t i=0; i<4; i++) {
            Serial.print(speeds[i]);
            Serial.print(' ');
        }
        Serial.println();
    }

    //delay(500);
}
