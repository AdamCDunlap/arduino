

#define runEvery(t) for (static typeof(t) _lasttime;\
                         (typeof(t))((typeof(t))millis() - _lasttime) > (t);\
                         _lasttime += (t))

void setup() {
    Serial.begin(115200);
}

void loop() {

    // char version
    runEvery((uint16_t) 250) {
        Serial.print("250 ms Current time: ");
        Serial.println(millis());
    }

    // int version
    runEvery(5000) {
        Serial.print("five seconds Current time: ");
        Serial.println(millis());
    }

    // long version
    runEvery(10ul * 60ul * 1000ul) { // ten minutes
        Serial.print("Ten minutes are up! Current time: ");
        Serial.println(millis());
    }

    delay(5);
}
