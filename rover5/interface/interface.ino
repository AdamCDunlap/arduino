/*
***Hardware setup***

arduino 1 -- motor controller interface | arduino 2 -- logic:
  00 - serial                           |   00 - serial
  01 - serial                           |   01 - serial
  02 - encoder1A                        |   02 - 
  03 - encoder1B                        |   03 - 
  04 - encoder2A                        |   04 - ultrasonic
  05 - mtr1pwm                          |   05 - 
  06 - mtr1dir                          |   06 - 
  07 - encoder2B                        |   07 - ultrasonic
  08 - encoder3A                        |   08 - 
  09 - mtr2pwm                          |   09 - turret servo
  10 - mtr3pwm                          |   10 - 
  11 - mtr4pwm                          |   11 - 
  12 - encoder3B                        |   12 - 
  13 - mtr2dir                          |   13 - 
a 14 - encoder4A                        | a 14 - mtr1current
n 15 - encoder4B                        | n 15 - mtr2current
a 16 - mtr3dir                          | a 16 - mtr3current
l 17 - mtr4dir                          | l 17 - mtr4current
o 18 - i2c communication                | o 18 - i2c
g 19 - i2c                              | g 19 - i2c
*/

#include <Wire.h>
#include "runmtrs.h"
#include "readencoders.h"

void setup();
void loop();

void setup() {
    Serial.begin(115200);

    Wire.begin(12);

    Serial.println("I'm alive");
    runmtrs::setup();
    readencoders::setup();
}

void loop() {
    runmtrs::loop();
    readencoders::loop();
    static unsigned long lastPrintTime = 0;
    if (millis() - lastPrintTime > 200) {
        char buf[137];
        sprintf(buf, "\r1spd:%4d 2spd:%4d 3spd:%4d 4spd:%4d\t"
                       "1dst:%11ld 2dst:%11ld 3dst:%11ld 4dst:%11ld",
                        mtrspds[0],mtrspds[1],mtrspds[2],mtrspds[3],
                        encdists[0],encdists[1],encdists[2],encdists[3]);
        Serial.print(buf);
    }

}
