#include <Rover5.h>
#include <Wire.h>

Rover5 bot(12);

void setup() {
    Serial.begin(115200);
    bot.begin();
}

long ticks[4];
long tmp;

void loop() {
    //bot.Run(100, 0, 0, 0);
    //delay(500);
    //bot.Run(-100, 0, 0, 0);
    //delay(500);

    //bot.Run(0, 100, 0, 0);
    //delay(500);
    //bot.Run(0, -100, 0, 0);
    //delay(500);

    //bot.Run(0, 0, 100, 0);
    //delay(500);
    //bot.Run(0, 0, -100, 0);
    //delay(500);

    //bot.Run(0, 0, 0, 100);
    //delay(500);
    //bot.Run(0, 0, 0, -100);
    //delay(500);

    Serial.println("Forward");
    bot.GetTicks(ticks);
    tmp = ticks[Rover5::FL];
    while(abs(ticks[Rover5::FL]-tmp) < 800) {
        bot.Run(0, 100);
        bot.GetTicks(ticks);
    }

    Serial.println("Backward");
    bot.GetTicks(ticks);
    tmp = ticks[Rover5::FL];
    while(abs(ticks[Rover5::FL]-tmp) < 800) {
        bot.Run(0, -100);
        bot.GetTicks(ticks);
    }

    Serial.println("Right");
    bot.GetTicks(ticks);
    tmp = ticks[Rover5::FL];
    while(abs(ticks[Rover5::FL]-tmp) < 800) {
        bot.Run(100, 0);
        bot.GetTicks(ticks);
    }

    Serial.println("Left");
    bot.GetTicks(ticks);
    tmp = ticks[Rover5::FL];
    while(abs(ticks[Rover5::FL]-tmp) < 800) {
        bot.Run(-100, 0);
        bot.GetTicks(ticks);
    }

   // Serial.println("Diamond");
   // bot.GetTicks(ticks);
   // tmp = ticks[Rover5::FL];
   // while(abs(ticks[Rover5::FR]-tmp) < 800) {
   //     bot.Run(100, 100);
   //     bot.GetTicks(ticks);
   // }

   // bot.GetTicks(ticks);
   // tmp = ticks[Rover5::FL];
   // while(abs(ticks[Rover5::FL]-tmp) < 800) {
   //     bot.Run(-100, 100);
   //     bot.GetTicks(ticks);
   // }

   // bot.GetTicks(ticks);
   // tmp = ticks[Rover5::FL];
   // while(abs(ticks[Rover5::FR]-tmp) < 800) {
   //     bot.Run(-100, -100);
   //     bot.GetTicks(ticks);
   // }

   // bot.GetTicks(ticks);
   // tmp = ticks[Rover5::FL];
   // while(abs(ticks[Rover5::FL]-tmp) < 800) {
   //     bot.Run(100, -100);
   //     bot.GetTicks(ticks);
   // }

    Serial.println("Clockwise");
    bot.GetTicks(ticks);
    tmp = ticks[Rover5::FL];
    while(abs(ticks[Rover5::FL]-tmp) < 800) {
        bot.Run(0, 0, 100);
        bot.GetTicks(ticks);
    }

    Serial.println("Counterclockwise");
    bot.GetTicks(ticks);
    tmp = ticks[Rover5::FL];
    while(abs(ticks[Rover5::FL]-tmp) < 800) {
        bot.Run(0, 0, -100);
        bot.GetTicks(ticks);
    }
}
