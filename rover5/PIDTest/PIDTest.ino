#include <stdinout.h>
#include <Rover5.h>
#include <PID.h>
#include <Wire.h>

Rover5 bot;
PID botPID(-255, 255);

void setup() {
    Serial.begin(115200);
    Serial.println(F("Good morning!"));
    bot.begin();

    Serial.println(F("Bot began"));
}

void loop() {

    // 22,0,0 is lowest value for sustaining oscillation
    // 1.48,0,0 is highest value for almost no reverse
    static float P = 1.48, I = 0, D = 0;

    static long setpoint = 100;

    if (Serial.available()) { // User started typing something
        if        (Serial.peek() == 'p') {
            Serial.read();
            Serial.println(F("\r\nGive a P value"));
            scanf_P(PSTR("%f"), &P);
            Serial.println(P);
        } else if (Serial.peek() == 'i') {
            Serial.read();
            Serial.println(F("\r\nGive a I value"));
            scanf_P(PSTR("%f"), &I);
            Serial.println(I);
        } else if (Serial.peek() == 'd') {
            Serial.read();
            Serial.println(F("\r\nGive a D value"));
            scanf_P(PSTR("%f"), &D);
            Serial.println(D);
        } else if (Serial.peek() == 's') {
            Serial.read();
            Serial.println(F("\r\nGive a Setpoint"));
            scanf_P(PSTR("%ld"), &setpoint);
            Serial.println(setpoint);
        } else {
            Serial.println(F("\r\nEnter new PID values"));
            scanf_P(PSTR("%f %f %f"), &P, &I, &D);
            printf_P(PSTR("%f %f %f"), P, I, D);
        }
    }
    botPID.SetPID(P, I, D);

    long encoderDists[4];
    
    bot.GetTicks(encoderDists);
    int wheelspd = botPID.GetOutput(encoderDists[Rover5::FL], setpoint, micros());
    bot.Run(0, wheelspd);
    
    printf_P(PSTR("\rP:%2f I:%2f D:%2f curdist:%6ld setpoint:%6ld curspd:%4d "),
                   P, I, D, encoderDists[Rover5::FL], setpoint, wheelspd);
}
