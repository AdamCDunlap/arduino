#include <stdinout.h>
#include <Rover5.h>
#include <Wire.h>

Rover5 bot;

void setup() {
    Serial.begin(115200);
    Serial.println(F("Good morning!"));
    bot.begin();

    Serial.println(F("Bot began"));
}

void loop() {

    static long curspd = 0;

    if (Serial.available()) { // User started typing something
        if (Serial.read() == 's') {
            //Serial.read();
            Serial.println(F("\r\nGive a curspd"));
            scanf_P(PSTR("%ld"), &curspd);
            Serial.println(curspd);
        }
    }

    long encoderDists[4];
    
    bot.GetTicks(encoderDists);
    bot.Run(0,curspd);
    
    static unsigned long lastprinttime = 0;
    if (millis() - lastprinttime > 100) {
        printf_P(PSTR("curdist:%6ld curspd:%6ld \n "),
                       encoderDists[Rover5::FL], curspd);
        lastprinttime = millis();
    }
    
}
