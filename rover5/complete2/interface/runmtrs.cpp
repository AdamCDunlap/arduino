#include "runmtrs.h"

void i2cReceive(int numBytes);

volatile bool receivedMessage = false;
volatile int receivedBytes;

void runmtrs::setup() {
    Wire.onReceive(i2cReceive);
}

void runmtrs::loop() {
    if (receivedMessage) {
        receivedMessage = false;
        int numBytes;
        cli();
        numBytes = receivedBytes;
        sei();
    
        if (numBytes != 8) {
            Serial.print(F("Received wrong number of bytes from master: "));
            Serial.print(numBytes);
            Serial.println();
            Serial.println(F("Received: "));
            for(int i=numBytes; i>0; i--) {
                while(Wire.available()<=0);
                Serial.println(Wire.read());
            }
        }
        else { // Received enough bytes
// second byte read (for each motor)is sign bit, so write that to the dir pin
// first byte read is the actual number. However, since it's two's compliment,
//  we need to negate it before using
            bool dir;
            uint8_t spd;

            for (uint8_t i=0; i<4; i++) {
                spd = Wire.read();
                dir = !!Wire.read();
                spd = dir? -spd : spd;
                digitalWrite(mtrdirPins[i], dir);
                analogWrite (mtrpwmPins[i], spd);

                Serial.print(F("Mtr ")); Serial.print(i); Serial.print(F(": "));
                Serial.write(dir? '-' : ' '); Serial.print(spd);
                Serial.println();

            }
        }
    }

}

// Interrupt, so must exit quickly
void i2cReceive(int numBytes) {
    receivedMessage = true;
    receivedBytes = numBytes;
}
