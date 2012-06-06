void setup() {
    Serial.begin(115200);
}

void loop() {
    Serial.print(analogRead(A5));
    Serial.print('\t');
    Serial.print(analogRead(A4));
    Serial.println();
    delay(200);
}
