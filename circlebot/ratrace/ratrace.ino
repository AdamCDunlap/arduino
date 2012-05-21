#include <CircleBot.h>
#include <AFMotor.h>

CircleBot bot(1, 3, 4);

void setup() {
    Serial.begin(9600);
    delay(3000);
    bot.Move(0, 1000);
    delay(20000);
    bot.Move(-500, 1000);
    delay(20000);
    bot.Move(1000, 1000);
    delay(20000);
    bot.Move(1000, -1000);
    delay(20000);
    bot.Move(0,-1000);
    delay(50000);
    bot.Move(0,0);
}

void loop() {
    
}
