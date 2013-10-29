
#include <MudduinoBot.h>


void MudduinoBot::motorEn() {
    digitalWrite(len, HIGH);
    digitalWrite(ren, HIGH);
}
void MudduinoBot::initMotors() {
    digitalWrite(lplus, LOW);
    digitalWrite(lminus, LOW);
    digitalWrite(len, LOW);
    digitalWrite(rplus, LOW);
    digitalWrite(rminus, LOW);
    digitalWrite(ren, LOW);

    pinMode(lplus, OUTPUT);
    pinMode(lminus, OUTPUT);
    pinMode(len, OUTPUT);
    pinMode(rplus, OUTPUT);
    pinMode(rminus, OUTPUT);
    pinMode(ren, OUTPUT);
}
void MudduinoBot::halt() {
    motorEn();
    digitalWrite(lplus, LOW);
    digitalWrite(lminus, LOW);
    digitalWrite(rplus, LOW);
    digitalWrite(rminus, LOW);
}
void MudduinoBot::forward() {
    motorEn();
    digitalWrite(lplus, HIGH);
    digitalWrite(lminus, LOW);
    digitalWrite(rplus, HIGH);
    digitalWrite(rminus, LOW);
}
void MudduinoBot::backward() {
    motorEn();
    digitalWrite(lplus, LOW);
    digitalWrite(lminus, HIGH);
    digitalWrite(rplus, LOW);
    digitalWrite(rminus, HIGH);
}
void MudduinoBot::turnL() {
    motorEn();
    digitalWrite(lplus, LOW);
    digitalWrite(lminus, HIGH);
    digitalWrite(rplus, HIGH);
    digitalWrite(rminus, LOW);
}
void MudduinoBot::turnR() {
    motorEn();
    digitalWrite(lplus, HIGH);
    digitalWrite(lminus, LOW);
    digitalWrite(rplus, LOW);
    digitalWrite(rminus, HIGH);
}
void MudduinoBot::move(int fwd, int turn) {
    int leftPow = fwd + turn;
    int rightPow = fwd - turn;
    leftPow = constrain(leftPow, -255, 255);
    rightPow = constrain(rightPow, -255, 255);
    //Serial.print("Left: ");
    //Serial.print(leftPow);
    //Serial.print("\tRight: ");
    //Serial.print(rightPow);
    //Serial.println();
    analogWrite(len, abs(leftPow));
    analogWrite(ren, abs(rightPow));

    if (leftPow >= 0) {
        digitalWrite(lminus, LOW);
        digitalWrite(lplus, HIGH);
    }
    else {
        digitalWrite(lminus, HIGH);
        digitalWrite(lplus, LOW);
    }
    if (rightPow >= 0) {
        digitalWrite(rplus, HIGH);
        digitalWrite(rminus, LOW);
    }
    else {
        digitalWrite(rplus, LOW);
        digitalWrite(rminus, HIGH);
    }
}

void MudduinoBot::initServo() {
    serv.attach(serv_pin);
}
void MudduinoBot::setServo(int x) {
    serv.write(x+5);
}
void MudduinoBot::setServoRaw(int x) {
    serv.write(x);
}

void MudduinoBot::initSensors() {
    pinMode(dist_pin, INPUT);
    pinMode(light_pin, INPUT);
    pinMode(reflect_pin, INPUT);
}
int MudduinoBot::getDist() {
    return analogRead(dist_pin);
}
int MudduinoBot::getLight() {
    return analogRead(light_pin);
}
int MudduinoBot::getReflect() {
    return analogRead(reflect_pin);
}
bool MudduinoBot::onWhite() {
    return getReflect() < 700;
}
