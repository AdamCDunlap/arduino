// vi:ts=4:expandtab
#include <avr/pgmspace.h>
#define DEBUG

#ifdef DEBUG
#define DEBUG_VAR(x) Serial.print(#x ": "); Serial.println(x);
#else
#define DEBUG_VAR(x)
#endif

const unsigned int senseprob = 0.8 * 0xffff;
const unsigned int  moveprob = 0.6 * 0xffff;

const size_t width = 24;
const size_t height = 24;
const byte world[width][height] PROGMEM = {
    {B00001000, B11011111, B11110110},
    {B00100101, B01001111, B11010111},
    {B11111010, B11100000, B01010010},
    {B01101001, B00000011, B01011010},
    {B01101001, B01010001, B10111101},
    {B11000000, B11000110, B10001111},
    {B11001001, B00010010, B11001001},
    {B11000100, B00110100, B10000110},
    {B01100100, B10101010, B00011110},
    {B01101010, B01000100, B01011110},
    {B11100000, B01110111, B01001110},
    {B10100011, B01010100, B10011010},
    {B11010111, B00000110, B00011000},
    {B01011010, B11001111, B10010000},
    {B00101001, B00011111, B10001011},
    {B01111101, B10010101, B10100011},
    {B01010110, B00001101, B11000010},
    {B01111110, B10001101, B01101011},
    {B01110000, B01110000, B00000100},
    {B11100100, B10010110, B00010101},
    {B10010111, B01011100, B00110110},
    {B01100101, B00011101, B01010101},
    {B00010000, B10100000, B01100000},
    {B01100010, B11101111, B01101110},
};

unsigned int probs[width][height];

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

void updatesense(bool onSquare;) {
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
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            unsigned int s;
            if (i-y > height || i-y < 0 || j-x > width || j-x < 0) 
                s = 0;
            else
                s = moveprob * world[i-y][j-x] + (1-moveprob) * world[i][j];
        }
    }
}


void setup() {
    Serial.begin(115200);
    
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
    updatesense();
    updatemove();
}
