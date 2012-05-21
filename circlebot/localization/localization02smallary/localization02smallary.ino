// vi:ts=4:expandtab
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <CircleBot.h>
#include <AFMotor.h>
//#define DEBUG

#if 0
char dummy;
// Arduino puts all of its own malformed prototypes in here
#endif
#line 12 // Tell the compiler this is line 12 even though arduino adds some
         // things before it

// This may be changed to a fixed-point arithmetic class for speed and memory
typedef float prob_t;

// Function prototypes
bool getworldcell(size_t row, size_t column);
bool readsensor();
void updatesense(bool onSquare);
prob_t getprob(size_t y, size_t x);
void updatemove(char x, char y);
void setup();
void loop();

CircleBot bot(1, 3, 4);

#ifdef DEBUG
#define DEBUG_VAR(x) do {Serial.print(#x ": "); Serial.println(x);} while(false)
#define DEBUG_ARY(x, len) do {\
        Serial.print(#x ": {"); Serial.print(x[0]); \
        for(size_t i=0;i<len;i++){\
            Serial.print(", ");\
            Serial.print(x[i]);\
        } Serial.println();\
    }while(false)
#define DEBUG_2D_ARY(x, height, width) do {Serial.println(#x ": {");\
        for(size_t i=0;i<height;i++){\
            Serial.print(#x ": {");\
            for(size_t j=0;j<width;j++){\
                Serial.print(x[i][j]);\
                Serial.print(", ");\
            } Serial.println("},");\
        }\
    }while(false)
#else
#define DEBUG_VAR(x) do{} while(false)
#define DEBUG_ARY(x, len) do {} while(false)
#define DEUBG_2D_ARY(x, height, width) do {} while(false)
#endif


const prob_t senseprob = 0.8;

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

// Holds all of the probabilities for each square
prob_t probs[width][height];

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
    return analogRead(A0) > 400;
}

void updatesense(bool onSquare) {
    float total = 0.0;
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            probs[i][j] *= (((onSquare?1:0) *    senseprob) +
                           ((onSquare?0:1) * (1-senseprob)));
            total += static_cast<float>(probs[i][j]);
        }
    }

    // Normalize the probs so they add up to 1
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            probs[i][j] /= total;
        }
    }
}

prob_t getprob(size_t y, size_t x) {
    if (x > 0 && y > 0 && x < width && y < height) return probs[y][x];
    return 0;
}

void updatemove(char x, char y) {
    // newprobs stores the new values of probs. Since we're essentially doing 
    // random access to probs, we can't update it on the fly
    prob_t newprobs[width][height];
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            if (i-y > height || i-y < 0 || j-x > width || j-x < 0)  {
                newprobs[i][j] = 0;
            }
            else {
                newprobs[i][j] =
                  // Moved as expected
                  //0.6 * probs[i-y][j-x] +
                  0.6 * getprob(i-y, j-x) +
                  // Didn't move
                  //0.1 * probs[i][j] +
                  0.1 * getprob(i, j) +
                  // Moved 1 too far in x
                  //0.1 * probs[i-y][j-(x+1)] + 
                  0.1 * getprob(i-y, j-(x+1)) + 
                  // Moved 1 too far in y
                  //0.05 * probs[i-(y+1)][j-x] + 
                  0.05 * getprob(i-(y+1), j-x) + 
                  // Moved 1 too far in x and y
                  //0.025 * probs[i-(y+1)][j-(x+1)] + 
                  0.025 * getprob(i-(y+1), j-(x+1)) + 
                  // Moved in x but not in y
                  //0.05 * probs[i][j-x] + 
                  0.05 * getprob(i, j-x) + 
                  // Moved in y but not in x
                  //0.075 * probs[i-y][j] + 
                  0.075 * getprob(i-y, j) + 

                  0;
            }
        }
    }

    // Do a deep copy of newprob into probs
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            probs[i][j] = newprobs[i][j];
        }
    }
}


void setup() {
    Serial.begin(115200);
    randomSeed(analogRead(A5));
    
    prob_t initval = prob_t(1)/(width*height);

    DEBUG_VAR(initval);
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            probs[i][j] = initval;
        }
    }
}

void loop() {
    updatesense(readsensor());
    
    //int xmove = random(2) - 1; // Get random number from -1 to 1
    //int ymove = random(2) - 1;
    //bot.Move(xmove*500, ymove*500);
    char xmove = 0;
    char ymove = 0;
    switch(random(4)) {
        case 0: xmove = +1; break;
        case 1: ymove = +1; break;
        case 2: xmove = -1; break;
        case 3: ymove = -1; break;
    }
    bot.Move(xmove * 500, ymove * 500);
    delay(275); // Tested to go about 1 square
    Serial.println("done delaying");

    updatemove(xmove, ymove);
    Serial.println("updated move");

    //DEBUG_2D_ARY(probs, height, width);
}
