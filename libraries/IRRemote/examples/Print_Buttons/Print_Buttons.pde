#include <IRRemote.h>

void setup(){
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.begin(9600);
  irInit(2);
}

void loop() {
  String button = irGetButtonName();
  if (button != "noButton") {
    Serial.print("Button: ");
    Serial.println(button);
    Serial.println();
  }
  delay(500);
}
