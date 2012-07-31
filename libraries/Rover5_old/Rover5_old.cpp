#include <Rover5.h>

void Rover5::Rover5(uint8_t address)
    : interfaceAddress(address)
{

}

void Rover5::init() {
    Wire.begin();
}

// Populates input array with the newest encoder data
void Rover5::getEncoderTickss(long dists[4]) {
    static union {
        uint32_t curdists[4];
        uint8_t  distsbreakdown[16];
    };
    
    if(Wire.available() >= 16) {
        for (int8_t i=0; i<16; i++) {
            distsbreakdown[i] = Wire.read();
        }
        Wire.requestFrom(interfaceAddress, (uint8_t)16);
    }

    dists[BR] = +curdists[0];
    dists[FR] = -curdists[1];
    dists[BL] = +curdists[2];
    dists[FL] = -curdists[3];
}

void Rover5::run(int spds[4]) {
    run(spds[FL], spds[FR], spds[BL], spds[BR]);
}

void Rover5::run(int FLspd, int FRspd, int BLspd, int BRspd) {
    int spds[4] = {-BRspd, FRspd, -BLspd, FLspd};
    Wire.beginTransmission(interfaceAddress);
    Wire.write((uint8_t *)&newspds, sizeof(newspds));
    Wire.endTransmission();
}

void Rover5::idle() {

}
