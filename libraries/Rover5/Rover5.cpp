#include <Rover5.h>

Rover5::Rover5(uint8_t i2caddress)
    : interfaceAddress(i2caddress)
    {

}

void Rover5::begin() {
    Wire.begin();
    Wire.requestFrom(interfaceAddress, (uint8_t)16);

    for (size_t i=0; i<spdLogLen; i++) {
        while(!UpdateSpeeds());
    }
}


// Each speed is an int from -255 to 255
void Rover5::Run(int frontLeft, int frontRight, int backLeft, int backRight) {
    powers[FL] = frontLeft;
    powers[FR] = frontRight;
    powers[BL] = backLeft;
    powers[BR] = backRight;
    Run();
}
void Rover5::Run(int powers[4]) {
    memcpy(Rover5::powers, powers, sizeof(Rover5::powers));
    Run();
}
void Rover5::Run(int x, int y, int z) {
    powers[FL] = +y + x + z;
    powers[FR] = +y - x - z;
    powers[BL] = +y - x + z;
    powers[BR] = +y + x - z;

    Normalize4(powers, 255);

    Run();
}

void Rover5::Run(int power, mtrNum_t num) {
    powers[num] = power;
    Run();
}

// Runs motors using values stored in powers array
void Rover5::Run() {
    powers[BR] *= -1;
    powers[BL] *= -1;

    Wire.beginTransmission(interfaceAddress);
    Wire.write((uint8_t *)&powers, sizeof(powers));
    Wire.endTransmission();

    powers[BR] *= -1;
    powers[BL] *= -1;
}
// Populates the powers array with the current speeds of each motor
void Rover5::GetPowers(int powers[4]) {
    memcpy(powers, Rover5::powers, sizeof(Rover5::powers));
}

// Populates the ticks array with the current number of encoder ticks for
//  each motor
void Rover5::GetTicks(long ticks[4]) {
    UpdateSpeeds();
    memcpy(ticks, Rover5::ticks, sizeof(Rover5::ticks));

    Serial.print('[');
    Serial.print(ticks[0]);
    for (uint8_t i=1; i<4; i++) {
        Serial.print(",\t"); Serial.print(ticks[i]);
    }
    Serial.print("] = [");
    Serial.print(Rover5::ticks[0]);
    for (uint8_t i=1; i<4; i++) {
        Serial.print(",\t"); Serial.print(Rover5::ticks[i]);
    }
    Serial.println(']');
}

// Populates the speeds array with the current speed of each motor in
//  mills per second
void Rover5::GetSpeeds(int speeds[4]) {
    UpdateSpeeds();
    memcpy(speeds, Rover5::speeds, sizeof(Rover5::speeds));
}

// This version is async but the i2c library is not -- also, it fails if
//  other arduino is disconnected
//// Read the current distances from the interface arduino and calculates
////  the speeds
//bool Rover5::UpdateSpeeds() {
//    if (Wire.available() >= 16) {
//        uint8_t* ticksbreakdown = (uint8_t*)ticks;
//        for (uint8_t i=0; i<16; i++) {
//            ticksbreakdown[i] = Wire.read();
//        }
//        Wire.requestFrom(interfaceAddress, (uint8_t)16);
//
//        unsigned long curTime = micros();
//
//        unsigned long timesDiff = curTime - tickLogs.times[tickLogs.nextEntry];
//        for (uint8_t i=0; i<4; i++) {
//            // Difference in ticks from oldest entry to entry about to be put in
//            //  over difference in the times over the same
//            long ticksDiff =  ticks[i] - tickLogs.ticks[tickLogs.nextEntry][i];
//            speeds[i] = 10000000.0 * (float)ticksDiff / (float)timesDiff;
//            //Serial.print(F("ck: ")); Serial.print(ticksDiff); Serial.print('\t');
//            //Serial.print(F("tm: ")); Serial.print(timesDiff); Serial.print('\t');
//        }
//        tickLogs.Put(ticks, curTime);
//
//        Serial.println(F("Bytes availalbe"));
//        return true;
//    }
//    Serial.println(F("Bytes not available"));
//    return false;
//}


// Read the current distances from the interface arduino and calculates
//  the speeds
bool Rover5::UpdateSpeeds() {

    {
        unsigned long endtime;
        unsigned long starttime;
        starttime = micros();
        Wire.requestFrom(interfaceAddress, (uint8_t)16);
        endtime = micros();
        Serial.print(F("requestFrom time: ")); Serial.print(endtime - starttime); Serial.print(' ');

    }

    if (Wire.available() < 16) { 
        Serial.println(F("Bytes not avilable"));
        return false;
    }

    uint8_t* ticksbreakdown = (uint8_t*)ticks;
    for (uint8_t i=0; i<16; i++) {
        ticksbreakdown[i] = Wire.read();
    }

    unsigned long curTime = micros();

    unsigned long timesDiff = curTime - tickLogs.times[tickLogs.nextEntry];
    Serial.print(F("tm: ")); Serial.print(timesDiff); Serial.print(' ');
    for (uint8_t i=0; i<4; i++) {
        // Difference in ticks from oldest entry to entry about to be put in
        //  over difference in the times over the same
        long ticksDiff =  ticks[i] - tickLogs.ticks[tickLogs.nextEntry][i];
        const float factor = 37699112.0;
        speeds[i] = (int)(factor * (float)ticksDiff / (float)timesDiff);
        Serial.print(F("ck")); Serial.print(i); Serial.print(F(": ")); Serial.print(ticksDiff); Serial.print(' ');
        //Serial.print(F("tm: ")); Serial.print(timesDiff); Serial.print('\t');
    }
    tickLogs.Put(ticks, curTime);
    Serial.print('|');

    return true;
}

void Rover5::Normalize4(int nums[4], int maximum) {
    int highest = abs(nums[1]);
    for (uint8_t i=1; i<4; i++) {
        int tmp = abs(i);
        if(tmp > highest) highest = tmp;
    }

    // If all are below the max, we don't need to do anything
    if (highest <= maximum) return;
    
    for (uint8_t i=0; i<4; i++) {
        nums[i] = (int)(( (long)nums[i] * (long)maximum)/(long)highest);
    }
}
