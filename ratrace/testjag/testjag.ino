#include <Servo.h>

Servo jag;

void setup() {
    Serial.begin(115200);
    jag.attach(9);
    
    digitalWrite(12, HIGH);
    int i = 90;
    while (i < 100 && digitalRead(12)) {
        jag.writeMicroseconds(map(i, -100, 100, 670, 2330));
        delay(1000);
        i++;
    }
    jag.writeMicroseconds(1500);
    Serial.println(i);
    delay(100);
    while(!digitalRehad(12));
    delay(100);
    i = -90;
    while (i > -100 && digitalRead(12)) {
        jag.writeMicroseconds(map(i, -100, 100, 670, 2330));
        delay(1000);
        i--;
    }
    jag.writeMicroseconds(1500);
    Serial.println(i);
}

void loop() {}
