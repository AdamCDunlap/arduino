#ifndef ROVER5_H
#define ROVER5_H
 
enum mtrNum { BR = 0, FR = 1, BL = 2, FL = 3 };

class Rover5 {
public:
    // Constructor, given the i2c address of the interface arduino
    Rover5(uint8_t address = 12);

    // Call once everything has been set up
    void Init();

    // Populates input array with the newest encoder data from i2c
    void GetEncoderTicks(long dists[4]);
    // Returns the encoder value for specified motor
    long GetEncoderTicks(mtrNum mtr);

    // Moves each motor with speeds given
    void RunMotors(int spds[4]) {
    void RunMotors(int FLspd, int FRspd, int BLspd, int BRspd) {
    void Run(int x, int y, int z);

    // Setter and getter for the i2c address of the interface arduino
    void SetAddress(uint8_t address) { interfaceAddress = address; }
    uint8_t GetAddress()             { return interfaceAddress; }
    
    // Call as often as possible for speed calculations
    void Idle();
private:
    uint8_t interfaceAddress;

    union  {
        uint32_t curdists[4];
        uint8_t  distsbreakdown[16];
    };
};
 
#endif //ROVER5_H
