

#define runEvery(t) for (static typeof(t) _lasttime;\
                         (typeof(t))((typeof(t))millis() - _lasttime) >= (t);\
                         _lasttime += (t))

void setup() {
    Serial.begin(115200);
}

void loop() {
    runEvery(500) {
        Serial.print("500 msec. time: ");
        Serial.println(millis());
    }
    runEvery((int8_t)125) {
        Serial.print("125 msec. time: ");
        Serial.println(millis());
    }
    runEvery(32766) {
        Serial.print("32.766 sec. Time: ");
        Serial.println(millis());
    }

    delay(5);
}
