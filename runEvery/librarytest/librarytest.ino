#include <RunEvery.h>

void setup() {
    Serial.begin(115200);
}

void loop() {
    runEvery(500) {
        Serial.print("Time: ");
        Serial.println(millis());
    }

    const int numThings = 2;
    static unsigned int period = 0;
    runEveryMicros(period) {
        static int i;
        switch(i) {
        case 0: period =  5000; break;
        case 1: period = 15000; break;
        }
        i = i < numThings? i+1 : 0;
    }
}
