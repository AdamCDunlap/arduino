#include <Rover5.h>

Rover5::Rover5(uint8_t i2caddress)
    : interfaceAddress(i2caddress)
    {

}

void Rover5::begin() {
    Wire.begin();
    //Wire.requestFrom(interfaceAddress, (uint8_t)16);

    for (size_t i=0; i<spdLogLen; i++) {
        while(!UpdateEncoders());
    }

    pos.x = 0;
    pos.y = 0;
    pos.angle = 0;
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

void Rover5::GetPowers(int outpowers[4]) {
    memcpy(outpowers, Rover5::powers, sizeof(Rover5::powers));
}

// Populates the ticks array with the current number of encoder ticks for
//  each motor
void Rover5::GetTicks(long outticks[4]) {
    memcpy(outticks, Rover5::ticks, sizeof(Rover5::ticks));
}

void Rover5::GetTickSpeeds(int outspeeds[4]) {
    memcpy(outspeeds, Rover5::speeds, sizeof(Rover5::speeds));
}

void Rover5::GetSpeeds(int outspeeds[4]) {
    for (uint8_t i=0; i<4; i++) {
        outspeeds[i] = speeds[i] * ticksToMills;
    }
}

void Rover5::GetDists(long dists[4]) {
    for (uint8_t i=0; i<4; i++) {
        dists[i] = ticks[i] * ticksToMills;
    }
}

long Rover5::GetDist() {
    return ((ticks[FL] + ticks[FR] + ticks[BL] + ticks[BR]) * ticksToMills)/4;
}

void Rover5::GetDist(long* xdist, long* ydist) {
    *xdist = ((+ticks[FL] -ticks[FR] -ticks[BL] +ticks[BR]) * ticksToMills)/4;
    *ydist = ((+ticks[FL] +ticks[FR] +ticks[BL] +ticks[BR]) * ticksToMills)/4;
    
}

void Rover5::GetPos(long* xpos, long* ypos) {
    *xpos = pos.x * ticksToMills;
    *ypos = pos.y * ticksToMills;
}

void Rover5::GetPos(long* xpos, long* ypos, unsigned int* angle) {
    *xpos = pos.x * ticksToMills;
    *ypos = pos.y * ticksToMills;
    *angle = pos.angle * ticksToMills;
}

bool Rover5::UpdateEncoders() {

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

    UpdateSpeeds(ticks);

    UpdatePosition();
    return true;
}

void Rover5::UpdateSpeeds(long ticks[4]) {
    unsigned long curTime = micros();

    unsigned long timesDiff = curTime - tickLogs.times[tickLogs.nextEntry];
    //Serial.print(F("tm: ")); Serial.print(timesDiff); Serial.print(' ');
    for (uint8_t i=0; i<4; i++) {
        // Difference in ticks from oldest entry to entry about to be put in
        //  over difference in the times over the same
        long ticksDiff =  ticks[i] - tickLogs.ticks[tickLogs.nextEntry][i];
        speeds[i] = (int)(1000000.0 * (float)ticksDiff / (float)timesDiff);
        //Serial.print(F("ck")); Serial.print(i); Serial.print(F(": ")); Serial.print(ticksDiff); Serial.print(' ');
        //Serial.print(F("tm: ")); Serial.print(timesDiff); Serial.print('\t');
    }
    tickLogs.Put(ticks, curTime);
    //Serial.print('|');
}

void Rover5::UpdatePosition() {
    // Variables used for integrating/dervitizing
    unsigned long curTime = micros();
    static unsigned long lastTime = curTime;
    unsigned int timeDiff = curTime - lastTime;
    lastTime = curTime;
    
    // "n" function to get the current rotational velocity

    // use K as defined in Ether's paper
    // "Kinematic Analysis of Four-Wheel Mecanum Vehicle"
    // http://www.chiefdelphi.com/media/papers/download/2722

    // 6.75 is wheel base in inches, 8.5 is track witdth
    // Factor of (1000/ticksToMills) on each one is to convert to ticks
    // dividing by 1000 makes it into milliradians
    // Units: ticks/milliradian
    // Ends up being 0.8090376273838012901584924638102813403418365325139869
    const double K = (((6.75*(1000/ticksToMills))/2 + (8.5*(1000/ticksToMills))/2) * 4)/1000;
    
    // no need to integrate the speeds when the distances are there
    //
    //long angVel = (+speeds[FL] -speeds[FR] +speeds[BL] -speeds[BR])/K;
    // Integrate angVel to get the angle
    // Devide by 1000000 to convert from microseconds to seconds
    //pos.angle += (angVel * (currentTime - lastTime)) / 1000000;
    
    pos.angle = ticksToMills * (double)(+ticks[FL] -ticks[FR] +ticks[BL] -ticks[BR])/K;

    
    // "r" function to get the field relative velocity and rotational velocity
    
    // the postfix r means it's robot relative
    // These are in ticks/second
    int xvelr = (+(long)speeds[FL] -(long)speeds[FR] -(long)speeds[BL] +(long)speeds[BR])/4;
    int yvelr = (+(long)speeds[FL] +(long)speeds[FR] +(long)speeds[BL] +(long)speeds[BR])/4;

    // Now rotate the vector
    float sinA = sin(pos.angle/1000.0);
    float cosA = cos(pos.angle/1000.0);
    float xvel = /*(int)*/(xvelr * cosA - yvelr * sinA);
    float yvel = /*(int)*/(xvelr * sinA + yvelr * cosA);

    // max val of xvel is 25000
    // ((2^32)-1)/25000 = 172,000, which would mean if timeDiff is more than
    // 172 milliseconds, it would overflow. So first divide timediff by 10
    // (not much is lost since micros() only has a precision of 4) so that
    // a full second and half can go by without overflow. More than that and
    // the user deserves to get wrong answers.
    //pos.x  += (((long)xvel * (long)(timeDiff/10))/100000);
    //pos.y  += (((long)yvel * (long)(timeDiff/10))/100000);
    pos.x  += (((float)xvel * (float)(timeDiff/10))/100000);
    pos.y  += (((float)yvel * (float)(timeDiff/10))/100000);

    //printf_P(PSTR("xvelr%5d yvelr%5d sinA %.3f xvel %.3f yvel %.3f pos.x % f pos.y % f pos.angle % f enc[FL] %ld enc[FR] %ld enc[BL] %ld enc[BR] %ld\r\n"),
    //                  xvelr,   yvelr,     sinA,     xvel,     yvel,    pos.x,    pos.y,    pos.angle,  ticks[FL],  ticks[FR],  ticks[BL],  ticks[BR]);
}

void Rover5::Normalize4(int nums[4], int maximum) {
    int highest = abs(nums[0]);
    for (uint8_t i=1; i<4; i++) {
        int tmp = abs(nums[i]);
        if(tmp > highest) highest = tmp;
    }

    // If all are below the max, we don't need to do anything
    if (highest <= maximum) return;
    
    for (uint8_t i=0; i<4; i++) {
        nums[i] = (int)(( (long)nums[i] * (long)maximum)/(long)highest);
    }
}
