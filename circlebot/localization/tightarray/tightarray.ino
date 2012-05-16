// vi:ts=4:expandtab
#include <avr/pgmspace.h>
#define DEBUG

const size_t width = 24;
const size_t height = 24;
const byte grid[width][height] PROGMEM = {
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

bool getcell(size_t row, size_t column) {
#ifdef DEBUG
    if (row < 0)      { Serial.print("Row must be >0 but is "); Serial.println(row); }
    if (row >= height) { Serial.print("Row must be <") Serial.print(height); Serial.print(" but is "); Serial.println(row); }
    if (column < 0) { Serial.print("Column must be >0 but is "); Serial.println(column); }
    if (column >= height) { Serial.print("Column must be <") Serial.print(width); Serial.print(" but is "); Serial.println(column); }
#endif
    return !!(pgm_read_byte(&grid[row][column/8])&(column%8));
}

void setup() {
    Serial.begin(115200);
    
    Serial.println(getcell(0,0));   // 0
    Serial.println(getcell(0,4));   // 1
    Serial.println(getcell(0,5));   // 0
    Serial.println(getcell(1,3));   // 0
    Serial.println(getcell(1,2));   // 1
    Serial.println(getcell(23,23)); // 0
    Serial.println(getcell(23,22)); // 1
}

void loop() {
  
}
