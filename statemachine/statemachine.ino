void setup() {
  pinMode(13, OUTPUT);
}

const byte sequence[] = {0, 2, 3, 1, 0, 3, 2, 0, 1};
const unsigned long delayTime = 1000;
const size_t sequencelen = sizeof(sequence)/sizeof(sequence[0]);

void loop() {
  static int statenum = 0;
  unsigned long curTime = millis();
  static unsigned long baseTime = 0;
  
  if (curTime - baseTime > delayTime) {
    baseTime = curTime;
    statenum = (statenum + 1) % sequencelen;
  }
  switch(sequence[statenum]) {
  case 0:
    digitalWrite(13, HIGH);
    break;
  case 1:
    digitalWrite(13, LOW);
    break;
  case 2:
    digitalWrite(13, HIGH);
    break;
  case 3:
    digitalWrite(13, LOW);
    break;
  }
}
