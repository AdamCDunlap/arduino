const int fiveV = A0;
const int GND = A1;
const int RATE = A2;
const int TEMP = A3;
const int STtwo = A4;
const int STone = A5;

//names of pins on gyro

long angle; //milliadegrees, between 0 and 360,000
long rate; //milliadegrees per millisecond
int rateReading; //0-1024
long lastMilliseconds; //time since epoch of last loopu
long milliseconds; //time since epoch of current loop
int looptime; //time per loop
int programTime; //time since start of program
long startTime; //time since epoch of end of setup
int lastSendTime; //time since epoch of last time a serial signal was sent

int maxRate = 3830; //rate gyro is turning when giving 5V in millirads per second
int offset = 509; //subtracted from rateReading
//calibrate these

void setup(){
  Serial.begin(9600);
  pinMode(fiveV, OUTPUT);
  pinMode(GND, OUTPUT);
  pinMode(RATE, INPUT);
  pinMode(TEMP, INPUT);
  pinMode(STtwo, OUTPUT);
  pinMode(STone, OUTPUT);
  
  
  digitalWrite(fiveV, HIGH); //DO NOT CHANGE - WILL DESTROY GRYOSCOPE
  digitalWrite(GND, LOW); //DO NOT CHANGE - WILL DESTROY GYROSCOPE
  digitalWrite(STtwo, LOW);
  digitalWrite(STone, LOW);
  startTime = millis();
}


void loop(){
  milliseconds = millis(); //set current time
  programTime = millis() - startTime;
  int bytesavailable = Serial.available();
  while (bytesavailable >= 4 ){
    maxRate = Serial.read() | (Serial.read() << 8);
    offset = Serial.read() | (Serial.read() << 8);
    bytesavailable = Serial.available();
  }
  
  rateReading = analogRead(RATE);
  rate = (long (maxRate)*(rateReading-offset)) >> 9; //Converts 0-1024 to about (-maxrate)-(maxrate)
  angle += rate*looptime/1000; //integrate rate to get angle
  while (angle < 0){
    angle += 6283; //Normalize Angle,
  }
  angle %= 6283; //0 <= angle < 360000
  if (milliseconds - lastSendTime > 50){
    Serial.write((unsigned char*)&angle, sizeof(angle)); //Sends angle as binary data
    Serial.write((unsigned char*)&rate, sizeof(rate)); //Sends rate as binary data
    //Serial.write((unsigned char*)&looptime, sizeof(looptime));
    //Serial.write((unsigned char*)&maxRate, sizeof(maxRate));
    //Serial.write((unsigned char*)&offset, sizeof(offset));
    //Serial.write((unsigned char*)&bytesavailable, 2);
    //while (milliseconds - lastMilliseconds < 50) {
    //    milliseconds = millis();
    //}
    lastSendTime = milliseconds;
  }
  looptime = milliseconds - lastMilliseconds; // figure out how long this loop took.
  lastMilliseconds = milliseconds;
}
