#include <Rover5.h>
#include <Wire.h>
#include <stdinout.h>

//#define DEBUG

#ifdef DEBUG
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
#else // DEBUG
#define TIME_OF(nm)
#endif

Rover5 bot;

void setup() {
    Serial.begin(115200);
    TIME_OF(F("hello")) Serial.println(F("Hello!"));
    bot.begin();
    Serial.println(F("Bot begun"));
}

void loop() {
    static int x = 0, y = 0, rotation = 0;

    if (Serial.available()) {
        Serial.println(F("Give new values in the format: <x> <y> <rotation>"));
        scanf_P(PSTR("%d %d %d"), &x, &y, &rotation);
    }
    TIME_OF(F("Run")) bot.Run(x, y, rotation);

    int speeds[4];
    long xdist, ydist;
    unsigned int botangle;
    TIME_OF(F("update")) bot.UpdateEncoders();
    TIME_OF(F("Get")) {
        bot.GetSpeeds(speeds);
        bot.GetPos(&xdist, &ydist, &botangle);
    }
    
    TIME_OF(F("Print")) {
        Serial.print(F("speeds: "));
        for (uint8_t i=0; i<4; i++) {
            Serial.print(speeds[i]);
            Serial.print(' ');
        }
        printf_P(PSTR("Spds: x:%4d y:%4d z:%4d Dist: x: %ld y: %ld z: %u"), x, y, rotation, xdist, ydist, botangle);
        Serial.println();
    }
}
