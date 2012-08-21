#include <Rover5.h>
#include <PID.h>
#include <Wire.h>

Rover5 bot;
//PID botPID[4];
//PID botPID;
PID_base<double, double> mypid;

void setup() {
    Serial.begin(115200);
    bot.begin();
}

void loop() {

}

