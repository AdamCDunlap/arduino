#include <Rover5.h>

Rover5::Rover5(uint8_t i2caddress)
    : interfaceAddress(i2caddress)
    {

}

void Rover5::begin() {
    Wire.begin();
    //Wire.requestFrom(interfaceAddress, (uint8_t)16);

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

    // map {-255,255} to {-255,-25}, {0}, and {25,255}
    for (uint8_t i=0; i<4; i++) {
        if (powers[i] < 0) {
            // Cast to unsigned for the multiplication and division,
            //  but then cast back before the subtraction
            powers[i] = (int)(((unsigned int)(powers[i] + 255) *
                               (unsigned int)(255-minPower)) / 255) - 255;
        } else if (powers[i] > 0) {
            powers[i] = (int)(((unsigned int)powers[i] *
                               (unsigned int)(255-minPower)) / 255) + minPower;
        }
        // If it's 0, keep it 0
    }

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
}

// Populates the speeds array with the current speed of each motor in
//  mills per second
void Rover5::GetSpeeds(int speeds[4]) {
    UpdateSpeeds();
    memcpy(speeds, Rover5::speeds, sizeof(Rover5::speeds));
}

void Rover5::GetDists(long dists[4]) {
    UpdateSpeeds();
    for (uint8_t i=0; i<4; i++) {
        dist[i] = ticks[i] * ticksToMills;
    }
}

long Rover5::GetDist() {
    UpdateSpeeds();
    long yticks = 0;
    for (uint8_t i=0; i<4; i++) {
        yticks += (ticks[i]/4);
    }
    return yticks * ticksToMills;
}

void Rover5::GetDist(long* xdist, long* ydist) {
    UpdateSpeeds();
    long yticks = +ticks[FL]/4 +ticks[FR]/4 +ticks[BL]/4 +ticks[BR]/4;
    long xticks = +ticks[FL]/4 -ticks[FR]/4 -ticks[BL]/4 +ticks[BR]/4;

    *xdist = xticks * ticksToMills;
    *ydist = xticks * ticksToMills;
}

void Rover5::GetDist(long* xdist, long* ydist, long* rotation) {
    GetDist(xdist, ydist); // This calls updatespeeds
    
    long rotationTicks = +ticks[FL]/4 -ticks[FR]/4 +ticks[BL]/4 -ticks[BR]/4;
}

// Read the current distances from the interface arduino and calculates
//  the speeds
bool Rover5::UpdateSpeeds() {

    {
        //unsigned long endtime;
        //unsigned long starttime;
        //starttime = micros();
        Wire.requestFrom(interfaceAddress, (uint8_t)16);
        //endtime = micros();
        //Serial.print(F("requestFrom time: "));
        //Serial.print(endtime - starttime);
        //Serial.print(' ');

    }

    if (Wire.available() < 16) { 
        //Serial.println(F("Bytes not avilable"));
        return false;
    }

    uint8_t* ticksbreakdown = (uint8_t*)ticks;
    for (uint8_t i=0; i<16; i++) {
        ticksbreakdown[i] = Wire.read();
    }
    ticks[FR] *= -1;
    ticks[FL] *= -1;

    unsigned long curTime = micros();

    unsigned long timesDiff = curTime - tickLogs.times[tickLogs.nextEntry];
    //Serial.print(F("tm: ")); Serial.print(timesDiff); Serial.print(' ');
    for (uint8_t i=0; i<4; i++) {
        // Difference in ticks from oldest entry to entry about to be put in
        //  over difference in the times over the same
        long ticksDiff =  ticks[i] - tickLogs.ticks[tickLogs.nextEntry][i];
        const float factor = 37699112.0;
        speeds[i] = (int)(factor * (float)ticksDiff / (float)timesDiff);
        //Serial.print(F("ck")); Serial.print(i); Serial.print(F(": ")); Serial.print(ticksDiff); Serial.print(' ');
        //Serial.print(F("tm: ")); Serial.print(timesDiff); Serial.print('\t');
    }
    tickLogs.Put(ticks, curTime);
    //Serial.print('|');

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
