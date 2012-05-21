#include <CircleBot.h>
#include <AFMotor.h>

CircleBot bot(1, 3, 4);

bool readsensor() {
    return analogRead(A0) > 400;
}

void setup() {
    Serial.begin(115200);
    

}

void loop() {
    while(!Serial.available());
    while(Serial.available()) Serial.read();
    bool startval = readsensor();
    unsigned long starttime = millis();
    bot.Move(500, 0);
    while (startval == readsensor());
    unsigned long elapsedtime = millis() - starttime;
    bot.Move(0, 0);
    Serial.println(elapsedtime);
}
