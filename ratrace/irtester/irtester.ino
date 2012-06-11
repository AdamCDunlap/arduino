#include <RunningMedian.h>

RunningMedian rightmed;
RunningMedian leftmed;

void setup() {
    Serial.begin(115200);
}

void loop() {
    rightmed.add(analogRead(A5));
    analogRead(A4);
    int rightval = rightmed.getMedian();
    Serial.print(rightval);
    Serial.print("\t=");
    Serial.print(43.89432 - 0.12038 * rightval + 0.0001038 * rightval * rightval);
    Serial.print("in\t");
    int leftval = leftmed.getMedian();
    Serial.print(leftval);
    Serial.print("\t=");
    Serial.print(43.89432 - 0.12038 * leftval + 0.0001038 * leftval * leftval);
    Serial.print("in\t");
    Serial.println();
    delay(100);
    leftmed.add(analogRead(A4));
    analogRead(A4);
    delay(100);
}
