#include <Wire.h>
#include <Servo.h>

static uint8_t interfaceAddress = 12;
void moveMotors(int spds[4]);
void getEncoders(long dists[4]);

enum mtrNum { BR = 0, FR = 1, BL = 2, FL = 3 };

void setup() {
    Serial.begin(115200);
    Wire.begin();

}

void loop() {
    int mtrSpds[4];
    long mtrDists[4];
    getEncoders(mtrDists);
    

    moveMotors(mtrSpds);
}

void moveMotors(int spds[4]) {
    int newspds[4] = {-spds[BR], spds[FR], -spds[BL], spds[FL]};
    Wire.beginTransmission(interfaceAddress);
    Wire.write((uint8_t *)&newspds, sizeof(newspds));
    Wire.endTransmission();
}

// Populates input array with the newest encoder data
void getEncoders(long dists[4]) {
    static union {
        uint32_t curdists[4];
        uint8_t  distsbreakdown[16];
    };
    static int numReceived = 0;
    
    while(Wire.available() >= 4) {
        for (uint8_t i=0; i<4; i++) {
            distsbreakdown[numReceived+4-i] = Wire.read();
        }
    }

    if (numReceived >= 4) {
        numReceived -= 4;
        // Make another request
        Wire.requestFrom(interfaceAddress, (uint8_t)16);
    }

    dists[BR] = -curdists[0];
    dists[FR] = +curdists[1];
    dists[BL] = -curdists[2];
    dists[FL] = +curdists[3];
}
