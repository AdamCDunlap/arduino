// vi:ts=4:expandtab
#include <avr/pgmspace.h>
#include <CircleBot.h>
#include <AFMotor.h>
#define DEBUG

CircleBot bot(1, 3, 4);

#ifdef DEBUG
#define DEBUG_VAR(x) Serial.print(#x ": "); Serial.println(x);
#else
#define DEBUG_VAR(x)
#endif

const float senseprob = 0.8;
const float  moveprob = 0.6;

#line 18

const size_t width = 16;
const size_t height = 16;
const byte world[width][height] PROGMEM = {
    {B00001000, B11011111},
    {B00100101, B01001111},
    {B11111010, B11100000},
    {B01101001, B00000011},
    {B01101001, B01010001},
    {B11000000, B11000110},
    {B11001001, B00010010},
    {B11000100, B00110100},
    {B01100100, B10101010},
    {B01101010, B01000100},
    {B11100000, B01110111},
    {B10100011, B01010100},
    {B11010111, B00000110},
    {B01011010, B11001111},
    {B00101001, B00011111},
    {B01111101, B10010101},
};

float probs[width][height];

bool getworldcell(size_t row, size_t column) {
#ifdef DEBUG
    if (row < 0)      { Serial.print("Row must be >0 but is "); Serial.println(row); }
    if (row >= height) { Serial.print("Row must be <"); Serial.print(height); Serial.print(" but is "); Serial.println(row); }
    if (column < 0) { Serial.print("Column must be >0 but is "); Serial.println(column); }
    if (column >= height) { Serial.print("Column must be <"); Serial.print(width); Serial.print(" but is "); Serial.println(column); }
#endif
    return !!(pgm_read_byte(&world[row][column/8])&(column%8));
}

bool readsensor() {
    int raw = analogRead(A0);
    return raw > 400;
}

void updatesense(bool onSquare) {
    float total = 0.0;
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            probs[i][j] = probs[i][j] * (onSquare * senseprob + (!onSquare) * (1-senseprob));
            total += probs[i][j];
        }
    }
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            probs[i][j] /= total;
        }
    }
}

void updatemove(char x, char y) {
    float newprobs[width][height];
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            if (i-y > height || i-y < 0 || j-x > width || j-x < 0) 
                newprobs[i][j] = 0;
            else
                newprobs[i][j] = moveprob * probs[i-y][j-x] + (1-moveprob) * probs[i][j];
        }
    }
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            probs[i][j] = newprobs[i][j];
        }
    }
}


void setup() {
    Serial.begin(115200);
    randomSeed(analogRead(A5));
    
    Serial.println(getworldcell(0,0));   // 0
    Serial.println(getworldcell(0,4));   // 1
    Serial.println(getworldcell(0,5));   // 0
    Serial.println(getworldcell(1,3));   // 0
    Serial.println(getworldcell(1,2));   // 1
    Serial.println(getworldcell(23,23)); // 0
    Serial.println(getworldcell(23,22)); // 1

    float initval = 0xffff/(width*height);
    DEBUG_VAR(initval);
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            probs[i][j] = initval;
        }
    }
}

void loop() {
    updatesense(readsensor());
    
    int xmove = random(2) - 1; // Get random number from -1 to 1
    int ymove = random(2) - 1;
    bot.Move(xmove, ymove, 0); // don't rotate
    delay(500);
    updatemove(xmove, ymove);
}
