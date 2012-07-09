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
// first byte read (for each motor)is sign bit, so write that to the dir pin
// second byte read is the actual number. However, since it's two's compliment,
//  we need to negate it before using
            bool dir;
            uint8_t spd;

            dir = !!Wire.read();
            spd = Wire.read();
            spd = dir? -spd : spd;
            digitalWrite(mtr1dirPin, dir);
            analogWrite (mtr1pwmPin, spd);

            dir = !!Wire.read();
            spd = Wire.read();
            spd = dir? -spd : spd;
            digitalWrite(mtr2dirPin, dir);
            analogWrite (mtr2pwmPin, spd);

            dir = !!Wire.read();
            spd = Wire.read();
            spd = dir? -spd : spd;
            digitalWrite(mtr2dirPin, dir);
            analogWrite (mtr2pwmPin, spd);

            dir = !!Wire.read();
            spd = Wire.read();
            spd = dir? -spd : spd;
            digitalWrite(mtr2dirPin, dir);
            analogWrite (mtr2pwmPin, spd);
        }
    }

}

// Interrupt, so must exit quickly
void i2cReceive(int numBytes) {
    receivedMessage = true;
    receivedBytes = numBytes;
}
