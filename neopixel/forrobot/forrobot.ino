#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <RunEvery.h>
#include <stdinout.h>

void cycleRed();
void readyToShoot();

Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, 6, NEO_GRB + NEO_KHZ800);

void setup() {
    strip.begin();
    strip.show();
    Wire.begin(38);
    Serial.begin(115200);
}

void loop() {
    cycleRed();
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
        for (int j=0; j<strip.numPixels(); j++) strip.setPixelColor(j, r, g, b);
        strip.show();
        i = i >= 1020? 0 : i+1;

        printf_P(PSTR("i: %4u r: %3hhu g: %3hhu b: %3hhu\n"), i, r, g, b);
    }
}

void readyToshoot() {

}
