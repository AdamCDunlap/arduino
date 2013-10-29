#ifndef MUDDUINOBOT_H
#define MUDDUINOBOT_H

#include <Arduino.h>
#include <Servo.h>

class MudduinoBot {
public:
    MudduinoBot(int lplus_in = 9, int lminus_in = 8, int len_in = 6, int rplus_in = 7, int rminus_in = 12, int ren_in = 11, int servo_in = 10, int dist_in = 14, int light_in = 19, int reflect_in = 18)
        : lplus(lplus_in),  lminus(lminus_in), len(len_in), rplus(rplus_in), rminus(rminus_in), ren(ren_in), serv_pin(servo_in), dist_pin(dist_in), light_pin(light_in), reflect_pin(reflect_in)
    {}

    void begin() { initMotors(); initServo(); initSensors(); }

    void motorEn();
    void initMotors();
    void halt();
    void forward();
    void backward();
    void turnL();
    void turnR();

    void move(int fwd, int turn=0);

    void initServo();
    void setServo(int x);
    void setServoRaw(int x);

    void initSensors();
    int getDist();
    int getLight();
    int getReflect();
    bool onWhite();
private:
    const int lplus, lminus, len, rplus, rminus, ren, serv_pin, dist_pin, light_pin, reflect_pin;
    Servo serv;
};

#endif//MUDDUINOBOT_H
