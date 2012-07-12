#include <TimerOne.h>

byte picture[8] = {
    B01100110,
    B11100111,
    B11111111,
    B01111110,
    B01111110,
    B00111100,
    B00011000,
    B00011000,
}

const int clockPin = 9;
const int dataPin = 10;
const int latchPin = 11;

void setup() {
    TimerOne.initialize(1000);              //These two lines have the arduino automatically
    TimerOne.attachInterrupt(screenUpdate); //call screenUpdate every 1000 microseconds
}

void loop() {
    for (int i=0; i < 8; i++) {
        picture[i] = ~picture[i]; // Reverse each row
    }
    delay(500);
}

void screenUpdate() {
    static int curRow = 1; // Static means keeps value loop to loop

    //~_bv means the inverse bit value of curRow -- basically
    //~_bv(1) = B11111110, ~_bv(2) = B11111101, ~_bv(3) = B11111011..

    //invert because the rows are generally the negatives of the LEDs
    shiftOut(dataPin, clockPin, MSBFIRST, ~_bv(curRow));

    //shiftOut the data for the current row.
    shiftOut(dataPin, clockPin, MSBFIRST, picture[curRow]);
    curRow = (curRow + 1) % 8; //Increase curRow, and loop to 0 if it's 8
}
