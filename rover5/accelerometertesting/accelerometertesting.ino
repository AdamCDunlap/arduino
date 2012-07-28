#include <Wire.h>
#include <Servo.h>

void sendNumber(int num);
void moveMotors(int mtr1, int mtr2, int mtr3, int mtr4);

void setup() {
    Serial.begin(115200);
    Wire.begin();
    
}

void loop() {
    int frontRight, frontLeft, backRight, backLeft;
    
    static float yax = 0;
    int rawY = analogRead(A3);
    int mapY = map(rawY,107,507,-1000,1000);
    
    unsigned long currenttime = micros();
    static unsigned long lasttime;
    unsigned long dt = currenttime-lasttime;
    lasttime = currenttime;
    
    yax += ((float)mapY*(float)dt)/100000.0;
    static unsigned long printTime = 0;
    if (millis() > printTime) {
        char buf[50];
        sprintf(buf, "mapY: %8d rawY: %8d yax: ", mapY, rawY);
        Serial.print(buf);
        Serial.println(yax);
        //printTime += 200;
        
    }
    // long distY += yax*dt/1000;
    //
    int motorvals = constrain(-yax, -255, 255);
    
    frontRight =   motorvals;
     frontLeft =   motorvals;
     backRight =   motorvals;
      backLeft =   motorvals;
//    frontRight = frontLeft = backRight = backLeft = 200;
      
      
    moveMotors(-backRight, frontRight, -backLeft, frontLeft);

}


void sendNumber(int num) {
    uint8_t* breakdown = reinterpret_cast<uint8_t *>(&num);
    Wire.write(breakdown, 2);
    //Serial.println(*(breakdown+0), BIN);
    //Serial.println(*(breakdown+1), BIN);
}


void moveMotors(int mtr1, int mtr2, int mtr3, int mtr4) {
    Wire.beginTransmission(12);
    sendNumber(mtr1);
    sendNumber(mtr2);
    sendNumber(mtr3);
    sendNumber(mtr4);
    Wire.endTransmission();
}
