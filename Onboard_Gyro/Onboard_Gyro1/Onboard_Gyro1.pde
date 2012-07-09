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
long lastMilliseconds; //time since epoch of last loop
long milliseconds; //time since epoch of current loop
int time; //time per loop

long maxRate = 211; //rate gyro is turning when giving 5V in millidegrees per millisecond
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
  
}


void loop(){
  milliseconds = millis(); //set current time
  rateReading = analogRead(RATE);
  rate = (maxRate*(rateReading-offset)) >> 9; //Calibrate here. Should take in 0-1024 and output about -150,000 to 150,000
  angle += rate*time; //integrate rate to get angle
  while (angle < 0){
    angle += 360000L; //Normalize Angle,
  }
  angle %= 360000L; //0 <= angle < 360000
  
  //Serial.print("Angle: ");
  Serial.write((unsigned char*)&angle, sizeof(angle)); //Sends angle as binary data
  //Serial.print("\t Rate: ");
  //Serial.write(" ");
  Serial.write((unsigned char*)&rate, sizeof(rate)); //Sends rate as binary data
  Serial.write((unsigned char*)&time, sizeof(time));
  //Serial.print("\t Volts: ");
  //Serial.print(volts);
  //Serial.print("\t Loop Time: ");
  //Serial.write(" ");
  //Serial.write(milliseconds - lastMilliseconds);
  //Serial.write("\n");
  //while (milliseconds - lastMilliseconds < 50) {
  //    milliseconds = millis();
  //}
  time = milliseconds - lastMilliseconds; // figure out how long this loop took.
  lastMilliseconds = milliseconds;
}
