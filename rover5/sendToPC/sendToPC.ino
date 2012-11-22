// sendToPC.ino
#include <Rover5.h>
#include <Wire.h>
#include <stdinout.h>

Rover5 bot;

void printdata() {
    //       1 u4       1 s4  1 s4  1 s4  1 s4  1 s2   1 s2 1 s2  1 s2  1 s4   1 s4   1 s4  1 u4        1
    // Print 0<stmicros>1<fld>2<frd>3<bld>4<brd>5<flp>6<frp>7<blp>8<brp>9<xpos>A<ypos>B<ang>C<endmicros>D
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
    Serial.write((uint8_t *)&pows[Rover5::FL], 2);
    Serial.write('6');
    Serial.write((uint8_t *)&pows[Rover5::FR], 2);
    Serial.write('7');
    Serial.write((uint8_t *)&pows[Rover5::BL], 2);
    Serial.write('8');
    Serial.write((uint8_t *)&pows[Rover5::BR], 2);
    Serial.write('9');

    Serial.write((uint8_t *)&xpos, 4);
    Serial.write('A');
    Serial.write((uint8_t *)&ypos, 4);
    Serial.write('B');
    Serial.write((uint8_t *)&ang, 4);
    Serial.write('C');

    Serial.write((uint8_t *)&endmicros, 4);
    Serial.write('D');


    //printf("%10lu %4ld %4ld %4ld %4ld % 4d % 4d % 4d % 4d %4ld %4ld %5u %10lu\n",
    //    stmicros,
    //    encdsts[Rover5::FL],
    //    encdsts[Rover5::FR],
    //    encdsts[Rover5::BL],
    //    encdsts[Rover5::BR],
    //    pows[Rover5::FL],
    //    pows[Rover5::FR],
    //    pows[Rover5::BL],
    //    pows[Rover5::BR],
    //    xpos,
    //    ypos,
    //    ang,
    //    endmicros
    //);
        





    //Serial.print('\t');
    //Serial.print(stmicros, HEX);
    //Serial.print('\t');
    //Serial.print(encdsts[Rover5::FL], HEX);
    //Serial.print('\t');
    //Serial.print(encdsts[Rover5::FR], HEX);
    //Serial.print('\t');
    //Serial.print(encdsts[Rover5::BL], HEX);
    //Serial.print('\t');
    //Serial.print(encdsts[Rover5::BR], HEX);

    //Serial.print('\t');
    //Serial.print(pows[Rover5::FL], HEX);
    //Serial.print('\t');
    //Serial.print(pows[Rover5::FR], HEX);
    //Serial.print('\t');
    //Serial.print(pows[Rover5::BL], HEX);
    //Serial.print('\t');
    //Serial.print(pows[Rover5::BR], HEX);
    //Serial.print('\t');

    //Serial.print(xpos, HEX);
    //Serial.print('\t');
    //Serial.print(ypos, HEX);
    //Serial.print('\t');
    //Serial.print(ang, HEX);
    //Serial.print('\t');

    //Serial.print(endmicros, HEX);
    //Serial.print('\t');

    //Serial.println();
}

void mydelay(unsigned long time) {
    for (unsigned long starttime = millis(); millis() - starttime < time;) {
        printdata();
    }
}

void setup() {
    Serial.begin(115200);
    bot.begin();

    //Serial.print("begin"); // Provide start marker
    //Serial.write((uint8_t)13, 2); // Print number of things that will be printed

    // Text will be printed normally unless in <>
    // Then first character will be a u or an s representing if the value is
    //  signed or unsigned
    // Second character will be number of bytes
    // Rest of text will be name
    // Format string ends when newline is encountered

    //Serial.print(F("0<u4stmicros>1<s4fld>2<s4frd>3<s4bld>4<s4brd>5"
    //               "<s2flp>6<s2frp>7<s2blp>8<s2brp>9<s4xpos>A"
    //               "<s4ypos>B<s4ang>C<u4endmicros>D\n"));
}

void loop() {
    bot.Run(100, 0, 0);
    mydelay(500);
    bot.Run(0, 100, 0);
    mydelay(500);
    bot.Run(0, 0, 100);
    mydelay(500);
    bot.Run(0, 0, 0);
    mydelay(500);
}
