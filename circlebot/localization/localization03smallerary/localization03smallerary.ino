// vi:ts=4:expandtab
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <CircleBot.h>
#include <AFMotor.h>
#include <LEDArray.h>
#define DEBUG
#define LEDARRAY

#if 0
char dummy;
// Arduino puts all of its own malformed prototypes in here
#endif
#line 14 // Tell the compiler this is line 14 even though arduino adds some
         // things before it

// This may be changed to a fixed-point arithmetic class for speed and memory
typedef float prob_t;

// Function prototypes
bool getworldcell(size_t row, size_t column);
bool readsensor();
void updatesense(bool onSquare);
prob_t getprob(size_t y, size_t x);
void updatemove(char x, char y);
void printProbs();
void setup();
void loop();

CircleBot bot(1, 3, 4);
#ifdef LEDARRAY
LEDArray disp(9, 13, 10, 2);
#endif

#ifdef DEBUG
#define DEBUG_VAR(x, ...) do {Serial.print(#x ": "); Serial.println(x, ##__VA_ARGS__);} while(false)
#define DEBUG_ARY(x, len, ...) do {\
        Serial.print(#x ": {"); Serial.print(x[0]); \
        for(size_t i=0;i<len;i++){\
            Serial.print(", ");\
            Serial.print(x[i], ##__VA_ARGS__);\
        } Serial.println();\
    }while(false)
#define DEBUG_2D_ARY(x, height, width, ...) do {\
        for(size_t i=0;i<height;i++){\
            Serial.print(#x ": {");\
            for(size_t j=0;j<width;j++){\
                Serial.print(x[i][j] , __VA_ARGS__);\
                Serial.print(", ");\
            } Serial.println("},");\
        }\
    }while(false)
#define DEBUG_MSG(...) do{Serial.println(__VA_ARGS__);} while(false)
#define DEBUG_PRINT(...) do{Serial.print(__VA_ARGS__);} while(false)
#else
#define DEBUG_VAR(x, ...) do{} while(false)
#define DEBUG_ARY(x, len, ...) do {} while(false)
#define DEUBG_2D_ARY(x, height, width, ...) do {} while(false)
#define DEBUG_MSG(...)
#define DEBUG_PRINT(...)
#endif

const prob_t senseprob = .99;

const size_t width  = 8;
const size_t height = 8;
const byte world[height][(width+7)/8] PROGMEM = {
    {B00001000},
    {B00100101},
    {B11111010},
    {B01101001},
    {B01101001},
    {B11000000},
    {B11001001},
    {B11000100},
};

// Holds all of the probabilities for each square
prob_t probs[height][width];

bool getworldcell(size_t row, size_t column) {
#ifdef DEBUG
    if (row < 0)         { Serial.print("Row must be >0 but is ");
                           Serial.println(row); }
    if (row >= height)   { Serial.print("Row must be <"); Serial.print(height);
                           Serial.print(" but is "); Serial.println(row); }
    if (column < 0)      { Serial.print("Column must be >0 but is ");
                           Serial.println(column); }
    if (column >= width) { Serial.print("Column must be <");
                           Serial.print(width); Serial.print(" but is ");
                           Serial.println(column); }
#endif
    return !!(pgm_read_byte(&world[row][column/8]) & _BV(7-(column%8)));
}

bool readsensor() {
    return analogRead(A0) > 400;
}

void updatesense(bool onSquare) {
    float total = 0.0;
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            bool hit = onSquare==getworldcell(i, j);
            probs[i][j] *= (hit?1:0) *    senseprob +
                           (hit?0:1) * (1-senseprob);
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
    prob_t newprobs[height][width];
    float total = 0;
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            //if (i-y > height || i-y < 0 || j-x > width || j-x < 0)  {
            //    newprobs[i][j] = 0;
            //}
            //else {
                newprobs[i][j] =
                  // Moved as expected
//                  getprob(i-y, j-x) +
                  0.6 * getprob(i-y, j-x) +
                  // Didn't move
                  0.1 * getprob(i, j) +
                  // Moved 1 too far in x
                  0.1 * getprob(i-y, j-(x+1)) + 
                  // Moved 1 too far in y
                  0.05 * getprob(i-(y+1), j-x) + 
                  // Moved 1 too far in x and y
                  0.025 * getprob(i-(y+1), j-(x+1)) + 
                  // Moved in x but not in y
                  0.05 * getprob(i, j-x) + 
                  // Moved in y but not in x
                  0.075 * getprob(i-y, j) + 

                  0;

                total += newprobs[i][j];
            //}
        }
    }

    // Do a deep copy of newprob into probs while normalizing at the same time
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            probs[i][j] = newprobs[i][j]/total;
        }
    }
}

void printProbs() {
    #if defined LEDARRAY || defined DEBUG
    
    DEBUG_VAR(digitalRead(2));
    
    prob_t maximum = 0;
    for(size_t i=0; i<height; i++) {
        for(size_t j=0; j<width; j++) {
            maximum = max(maximum, probs[i][j]);
        }
    }
    maximum -= 0.005; // Give a little leeway
    DEBUG_VAR(maximum, 6);

    #ifdef LEDARRAY
    byte ledpic[8];
    #endif

    for(size_t i=0; i<height; i++){
        #ifdef DEBUG
        Serial.print("probs: {");
        #endif
        for(size_t j=0; j<width; j++){
            #ifdef DEBUG
            if (probs[i][j] > maximum) Serial.print('*');
            else Serial.print(' ');
            Serial.print(probs[i][j], 6);
            Serial.print(",");
            #endif
            #ifdef LEDARRAY
            bitWrite(ledpic[i], j, probs[i][j] > maximum);
            #endif
        }
        #ifdef DEBUG
        Serial.println("},");
        #endif
    }

    #ifdef LEDARRAY
    disp.drawPic(ledpic);
    #endif

    #endif // LEDARRAY || DEBUG
}




void setup() {
#ifdef DEBUG
    Serial.begin(115200);
    DEBUG_MSG("Compiled at " __TIME__ " on " __DATE__ " in " __FILE__);
#endif
    randomSeed(analogRead(A5));
    
    prob_t initval = prob_t(1)/(width*height);

    #ifdef DEBUG
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            DEBUG_PRINT(getworldcell(i, j));
        }
        DEBUG_MSG();
    }
    #endif
    DEBUG_VAR(initval, 8);
    for (size_t i=0; i<height; i++) {
        for (size_t j=0; j<width; j++) {
            probs[i][j] = initval;
        }
    }
    delay(1000);
}

void loop() {
    bool onBlack = readsensor();
//    bool onBlack = Serial.read()=='b'? true : false;
#ifdef DEBUG
    while(!Serial.available());
    switch(Serial.read()) {
    case '1': onBlack = true; break;
    case '0': onBlack = false; break;
    default: break;
    }
#endif
    DEBUG_VAR(onBlack);
    updatesense(onBlack);
    DEBUG_MSG("sensed");
    printProbs();
    
#ifdef DEBUG
    while(!Serial.available());
#endif
    

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
#ifdef DEBUG
    switch(Serial.read()) {
    case 'w': ymove=+1; xmove= 0; break;
    case 'a': ymove= 0; xmove=+1; break;
    case 's': ymove=-1; xmove= 0; break;
    case 'd': ymove= 0; xmove=-1; break;
    default: break;
    }
#endif
    bot.Move(xmove * 500, ymove * -500);
    DEBUG_PRINT(  "x="); DEBUG_PRINT(xmove);
    DEBUG_PRINT("\ty="); DEBUG_PRINT(ymove);
    DEBUG_MSG();

    delay(275); // Tested to go about 1 square
    bot.Move(0,0);

    updatemove(xmove, ymove);
    DEBUG_MSG("updated move");

    printProbs();
    //DEBUG_2D_ARY(probs, height, width, 8);
    //delay(1000);
}
