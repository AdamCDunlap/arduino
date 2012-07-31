#include <Wire.h>
#include <Servo.h>

void moveMotors(int spds[4]);
void getEncoders(long dists[4]);

static uint8_t interfaceAddress = 12;
enum mtrNum { BR = 0, FR = 1, BL = 2, FL = 3 };

void setup() {
    Serial.begin(115200);
    Wire.begin();

    // Reset the interface arduino
    pinMode(7, OUTPUT);
    digitalWrite(7, LOW);
    delay(10);
    pinMode(7, INPUT);
    delay(500);

    Wire.requestFrom(interfaceAddress, (uint8_t)16);

    char serialBuf[100];

    long mtrDists[4];
    int  mtrSpds[4] = {0,0,0,0};
    getEncoders(mtrDists);

    sprintf(serialBuf,"FRdst:%7ld FLdst:%7ld BRdst:%7ld BLdst:%7ld",
        mtrDists[FR], mtrDists[FL], mtrDists[BR], mtrDists[BL]);
    Serial.println(serialBuf);
    
    while (mtrDists[FL] == 0) {
        moveMotors(mtrSpds);
        mtrSpds[FL]++;
        getEncoders(mtrDists);

        //sprintf(serialBuf,"FRdst:%7ld FLdst:%7ld BRdst:%7ld BLdst:%7ld",
        //    mtrDists[FR], mtrDists[FL], mtrDists[BR], mtrDists[BL]);
        //Serial.println(serialBuf);

        delay(500);
    }
    Serial.print("FL min speed: "); Serial.println(mtrSpds[FL]);
    mtrSpds[FL] = 0;
    moveMotors(mtrSpds);

    sprintf(serialBuf,"FRdst:%7ld FLdst:%7ld BRdst:%7ld BLdst:%7ld",
        mtrDists[FR], mtrDists[FL], mtrDists[BR], mtrDists[BL]);
    Serial.println(serialBuf);

    while (mtrDists[FR] == 0) {
        moveMotors(mtrSpds);
        mtrSpds[FR]++;
        getEncoders(mtrDists);
        delay(500);
    }
    Serial.print("FR min speed: "); Serial.println(mtrSpds[FR]);
    mtrSpds[FR] = 0;
    moveMotors(mtrSpds);

    sprintf(serialBuf,"FRdst:%7ld FLdst:%7ld BRdst:%7ld BLdst:%7ld",
        mtrDists[FR], mtrDists[FL], mtrDists[BR], mtrDists[BL]);
    Serial.println(serialBuf);

    while (mtrDists[BL] == 0) {
        moveMotors(mtrSpds);
        mtrSpds[BL]++;
        getEncoders(mtrDists);
        delay(500);
    }
    Serial.print("BL min speed: "); Serial.println(mtrSpds[BL]);
    mtrSpds[BL] = 0;
    moveMotors(mtrSpds);

    sprintf(serialBuf,"FRdst:%7ld FLdst:%7ld BRdst:%7ld BLdst:%7ld",
        mtrDists[FR], mtrDists[FL], mtrDists[BR], mtrDists[BL]);
    Serial.println(serialBuf);

    while (mtrDists[BR] == 0) {
        moveMotors(mtrSpds);
        mtrSpds[BR]++;
        getEncoders(mtrDists);
        delay(500);
    }
    Serial.print("BR min speed: "); Serial.println(mtrSpds[BR]);
    mtrSpds[BR] = 0;
    moveMotors(mtrSpds);

    sprintf(serialBuf,"FRdst:%7ld FLdst:%7ld BRdst:%7ld BLdst:%7ld",
        mtrDists[FR], mtrDists[FL], mtrDists[BR], mtrDists[BL]);
    Serial.println(serialBuf);
}

void loop() {
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
    //static int numReceived = 0;
    
    while(Wire.available() >= 32) {
        // Throw away old data
        for (uint8_t i=0; i<16; i++) {
            Wire.read();
        }
    }

    if(Wire.available() >= 16) {
        //for (int8_t i=15; i>=0; i--) {
        for (int8_t i=0; i<16; i++) {
            distsbreakdown[i] = Wire.read();
            //numReceived++;
        }
        Wire.requestFrom(interfaceAddress, (uint8_t)16);
    }

    dists[BR] = +curdists[0];
    dists[FR] = -curdists[1];
    dists[BL] = +curdists[2];
    dists[FL] = -curdists[3];
}
