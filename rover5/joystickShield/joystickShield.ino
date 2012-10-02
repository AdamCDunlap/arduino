#include <Wire.h>
#include <Rover5.h>

#define DEBUG_VAR(x) do {Serial.print(#x ": "); Serial.print(x); Serial.print('\t'); } while (0)
Rover5 bot;

void setup() {
    Serial.begin(115200);
    bot.begin();
    
}

void loop() {

    int xraw = analogRead(0);
    int yraw = analogRead(1);

    int xval = map(xraw, 0, 1023, -255, +255);
    int yval = map(yraw, 0, 1023, -255, +255);

    if (abs(xval) > 10 || abs(yval) > 10) {
        bot.Run(xval, yval, 0);
    } else {
        bot.Run(0,0,0);
    }

    static unsigned long lastSerialTime = 0;
    if (millis() - lastSerialTime > 200) {
        char buf[128];
        sprintf(buf, "xval: %4d yval: %4d",
                      xval,     yval     );
        Serial.println(buf);
        lastSerialTime = millis();
    }
}
