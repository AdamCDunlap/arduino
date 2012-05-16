void setup(){
  Serial.begin(9600);
}

void loop(){
  Serial.print(Serial.read());
  Serial.print("\n");
  delay (50);
}
