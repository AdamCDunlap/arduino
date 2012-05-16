void setup(){
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Serial.begin(115200);
}

void loop(){
  Serial.write(PIND);
//  Serial.write(PINB);
}
