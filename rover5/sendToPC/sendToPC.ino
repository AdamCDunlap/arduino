// sendToPC.ino
#include <Rover5.h>
#include <Wire.h>
#include <stdinout.h>

Rover5 bot;

void printdata() {
    // 1 u4       1 s4  1 s4  1 s4  1 s4  1 s2   1 s2 1 s2  1 s2  1 s4   
    // 0<stmicros>1<fld>2<frd>3<bld>4<brd>5<flp>6<frp>7<blp>8<brp>9<xpos>
    // 1 s4   1 s4  1 s4    1 s4    1 s2  1 s2  1 s2  1 s2    u4        1
    // A<ypos>B<ang>C<xdist>D<ydist>E<fls>F<frs>G<bls>H<brs>I<endmicros>J
    uint32_t stmicros = micros();
    bot.UpdateEncoders();
    uint32_t endmicros = micros();
    int32_t encdsts[4];
    bot.GetTicks(encdsts);
    int16_t  pows[4];
    bot.GetPowers(pows);
    int32_t xpos, ypos;
    uint16_t ang;
    bot.GetPos(&xpos, &ypos, &ang);
    int32_t xdist, ydist;
    bot.GetDist(&xdist, &ydist);
    int16_t spds[4];
    bot.GetSpeeds(spds);

    Serial.write('0');
    Serial.write((uint8_t *)&stmicros, 4);
    Serial.write('1');
    Serial.write((uint8_t *)&encdsts[Rover5::FL], 4);
    Serial.write('2');
    Serial.write((uint8_t *)&encdsts[Rover5::FR], 4);
    Serial.write('3');
    Serial.write((uint8_t *)&encdsts[Rover5::BL], 4);
    Serial.write('4');
    Serial.write((uint8_t *)&encdsts[Rover5::BR], 4);
    Serial.write('5');
// 26
    Serial.write((uint8_t *)&pows[Rover5::FL], 2);
    Serial.write('6');
    Serial.write((uint8_t *)&pows[Rover5::FR], 2);
    Serial.write('7');
    Serial.write((uint8_t *)&pows[Rover5::BL], 2);
    Serial.write('8');
    Serial.write((uint8_t *)&pows[Rover5::BR], 2);
    Serial.write('9');
// 38
    Serial.write((uint8_t *)&xpos, 4);
    Serial.write('A');
    Serial.write((uint8_t *)&ypos, 4);
    Serial.write('B');
    Serial.write((uint8_t *)&ang, 4);
    Serial.write('C');
// 53
    Serial.write((uint8_t *)&xdist, 4);
    Serial.write('D');
    Serial.write((uint8_t *)&ydist, 4);
    Serial.write('E');
// 63
    Serial.write((uint8_t *)&spds[Rover5::FL], 2);
    Serial.write('F');
    Serial.write((uint8_t *)&spds[Rover5::FR], 2);
    Serial.write('G');
    Serial.write((uint8_t *)&spds[Rover5::BL], 2);
    Serial.write('H');
    Serial.write((uint8_t *)&spds[Rover5::BR], 2);
    Serial.write('I');
// 75
    Serial.write((uint8_t *)&endmicros, 4);
    Serial.write('J');
// 80
}

void mydelay(unsigned long time) {
    for (unsigned long starttime = millis(); millis() - starttime < time;) {
        printdata();
    }
}

void setup() {
    Serial.begin(115200);
    bot.begin();
}

size_t bufpos;
char buf[128];

int16_t x, y, z;
void loop() {
    if (Serial.available()) {
        buf[bufpos++] = Serial.read();
        if (bufpos >= 6) {
            memcpy(&x, &buf[0*sizeof(x)], sizeof(x));
            memcpy(&y, &buf[1*sizeof(y)], sizeof(y));
            memcpy(&z, &buf[2*sizeof(z)], sizeof(z));
            bufpos = 0;
        }
    }
    bot.Run(x, y, z);
    printdata();
}

