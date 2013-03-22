#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <RunEvery.h>
#include <stdinout.h>

void cycleRed();
void readyToShoot();
void getI2C(int numBytes);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, 6, NEO_GRB + NEO_KHZ800);

namespace RS {
    enum RobotState { NORMAL, HAVE_FRISBEE, DROPPING_FRISBEE, FIRING_FRISBEE, CLIMBING, CLIMBED };
    volatile uint8_t RS;
}

void setup() {
    strip.begin();
    strip.show();
    Wire.begin(38);
    Wire.onReceive(getI2C);
    Serial.begin(115200);
}

void loop() {
    switch((RS::RobotState)RS::RS) {
    // Add different functions in here for different states
    case RS::NORMAL: cycleRed(); break;
    case RS::HAVE_FRISBEE: cycleRed(); break;
    case RS::DROPPING_FRISBEE: cycleRed(); break;
    case RS::FIRING_FRISBEE: cycleRed(); break;
    case RS::CLIMBING: cycleRed(); break;
    case RS::CLIMBED: cycleRed(); break;
    }
}

void getI2C(int numBytes) {
    //if (numByte != 1) printf_P(PSTR("Received wrong number of bytes: %d\n"), numBytes);
    byte b = Wire.read();
    switch(b) {
    case 1: RS::RS = RS::NORMAL; break;
    case 2: if (RS::RS == RS::NORMAL) RS::RS = RS::HAVE_FRISBEE; break;
    case 3: if (RS::RS == RS::HAVE_FRISBEE) RS::RS = RS::NORMAL; break;
    case 4: RS::RS = RS::DROPPING_FRISBEE; break;
    case 5: RS::RS = RS::FIRING_FRISBEE; break;
    case 6: RS::RS = RS::CLIMBING; break;
    case 7: RS::RS = RS::CLIMBED; break;
    }
}

void cycleRed() {
    runEvery(10) {
        static unsigned int i = 0;
        static byte r = 255, g = 0, b = 0;
        // Always red. Blue up, blue down, green up, green down
        if (i < 255) ++b;
        else if (i < 510) --b;
        else if (i < 765) ++g;
        else if (i < 1020) --g;
        for (unsigned int j=0; j<strip.numPixels(); j++) strip.setPixelColor(j, r, g, b);
        strip.show();
        i = i >= 1020? 0 : i+1;

        printf_P(PSTR("i: %4u r: %3hhu g: %3hhu b: %3hhu\n"), i, r, g, b);
    }
}
