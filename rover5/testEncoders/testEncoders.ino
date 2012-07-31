#include <Wire.h>
#include <Servo.h>

void moveMotors(int spds[4]);
void getEncoders(long dists[4]);
void normalize4(int* a, int* b, int* c, int* d, int highest);

static uint8_t interfaceAddress = 12;
enum mtrNum { BR = 0, FR = 1, BL = 2, FL = 3 };

void setup() {
    Serial.begin(115200);
    Wire.begin();

    Wire.requestFrom(interfaceAddress, (uint8_t)16);
}

void loop() {
    int mtrSpds[4] = {0, 0, 0, 0};
    long mtrDists[4];
    getEncoders(mtrDists);
    
    int xraw = analogRead(0);
    int yraw = analogRead(1);

    int xval = map(xraw, 0, 1023, -255, +255);
    int yval = map(yraw, 0, 1023, -255, +255);

    if ((long)xval*(long)xval + (long)yval*(long)yval > 200) {

        mtrSpds[FR] = +yval - xval;
        mtrSpds[FL] = +yval + xval;
        mtrSpds[BR] = +yval + xval;
        mtrSpds[BL] = +yval - xval;

        normalize4(&mtrSpds[FL], &mtrSpds[BL], &mtrSpds[FR], &mtrSpds[BR], 255);

    }

    moveMotors(mtrSpds);


    static unsigned long lastSerialTime = 0;
    if (millis() - lastSerialTime > 200) {
        char buf[128]; // Length optained by experimentation (strlen+1)

        sprintf(buf, "xval:%4d yval:%4d "
        "FRspd:%4d FLspd:%4d BRspd:%4d BLspd:%4d "
        "FRdst:%7ld FLdst:%7ld BRdst:%7ld BLdst:%7ld",
        xval, yval,
        mtrSpds[FR], mtrSpds[FL], mtrSpds[BR], mtrSpds[BL], 
        mtrDists[FR], mtrDists[FL], mtrDists[BR], mtrDists[BL]);

        //Serial.print("Strlength: "); Serial.println(strlen(buf));
        Serial.println(buf);
        lastSerialTime = millis();
    }
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

void normalize4(int* a, int* b, int* c, int* d, int maximum) {
    int A = abs(*a);
    int B = abs(*b);
    int C = abs(*c);
    int D = abs(*d);
    int highest = max(A, B);
        highest = max(highest, C);
        highest = max(highest, D);

    //Serial.print("Highest: "); Serial.print(highest); Serial.print('\t');
     
    // If all are below the max, we don't need to do anything
    if (highest <= maximum) return;
    
    // The highest/2 factor is to round correctly
    //long roundval = highest/2;

    *a = (int)((((long)*a * (long)maximum) /*+ (long)roundval*/ )/(long)highest);
    *b = (int)((((long)*b * (long)maximum) /*+ (long)roundval*/ )/(long)highest);
    *c = (int)((((long)*c * (long)maximum) /*+ (long)roundval*/ )/(long)highest);
    *d = (int)((((long)*d * (long)maximum) /*+ (long)roundval*/ )/(long)highest);
}
