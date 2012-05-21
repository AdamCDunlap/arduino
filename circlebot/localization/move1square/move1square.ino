#include <CircleBot.h>
#include <AFMotor.h>

CircleBot bot(1, 3, 4);

void setup() {
    Serial.begin(115200);
}

void loop() {
    while(!Serial.available());
    while(Serial.available()) Serial.read();
    bot.Move(0, 500);
    delay(275);
    bot.Move(0,0);
}
