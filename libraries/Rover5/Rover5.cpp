#include <Rover5.h>

Rover5::Rover5(uint8_t i2caddress) : interfaceAddress(i2caddress)
    {

}

void Rover5::begin() {
    Wire.begin();
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
    memcpy(Rover5::powers, powers, 4);
    Run();
}
void Rover5::Run(int x, int y, int z) {
    powers[FL] = +y - x - z;
    powers[FR] = +y + x + z;
    powers[BL] = +y + x - z;
    powers[BR] = +y - x + z;

    Normalize4(powers, 255);

    Run(pows);
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
    memcpy(powers, Rover5::powers, 4);
}

// Populates the ticks array with the current number of encoder ticks for
//  each motor
void Rover5::GetTicks(long ticks[4]) {
    UpdateSpeeds();
    memcpy(ticks, Rover5::ticks, 4);
}

// Populates the speeds array with the current speed of each motor in
//  mills per second
void Rover5::GetSpeeds(int speeds[4]) {
    UpdateSpeeds();
    memcpy(speeds, Rover5::speeds, 4);
}

// Read the current distances from the interface arduino and calculates
//  the speeds
void Rover5::UpdateSpeeds() {
    if (Wire.available() >= 16) {
        uint8_t* ticksbreakdown = (uint8_t*)ticks;
        for (uint8_t i=0; i<16; i++) {
            ticksbreakdown[i] = Wire.read();
        }
        Wire.requestFrom(interfaceAddress, (uint8_t)16);
        
        for (uint8_t i=0; i<4; i++) {
            TicksLog<10>* curlog = &tickLogs[i];
            curlog.Put(ticks[i], millis());
            
            // TODO: Figure out factor and scale here
            // XXX: nextpos-2 won't work; needs to wrap around
            speeds[i] = 
            ((curlog.ticks[curlog.nextPos-2] - curlog.ticks[curlog.nextPos-1]) /
             (curlog.times[curlog.nextPos-2] - curlog.times[curlog.nextPos-1]));
        }
    }
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
