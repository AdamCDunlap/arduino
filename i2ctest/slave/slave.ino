#include <Wire.h>

uint8_t var[1];
void i2cRequest() {
    Wire.write((uint8_t *)var, 1);
}

void setup() {
    Wire.begin(12);
    Wire.onRequest(i2cRequest);
}
void loop() {}
