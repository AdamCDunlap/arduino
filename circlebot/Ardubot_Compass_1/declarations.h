

struct motorspeeds {int speed4; int speed3; int speed1;};
struct axis {int x; int y; int z;};

struct moveRobotOutputs {
    struct motorspeeds speeds;
    struct axis axis;
};

void processPulse1();
void processPulse2();
void processPulse();
void ISRRUTT_end();
void ISRRUTT_begin();
void ISRTHRO_end();
void ISRTHRO_begin();
void readReceiverBegin(int, int, int = 0);
unsigned int joyTimeRX();
unsigned int joyTimeLX();
unsigned int joyTimeRY();
unsigned int joyTimeLY();
int runMtr(AF_DCMotor, long);
struct moveRobotOutputs moveRobot(int, int, int, float = 0);

