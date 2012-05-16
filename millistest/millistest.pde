// This variable stores the current time returned by millis()
extern volatile unsigned long timer0_millis;

void setup() {
    Serial.begin(9600);

    cli(); //disable interrupts

    // Max unsigned long: 4,294,967,295 
    // Set the current time to 5 seconds before wrap around
    //   so we don't have to wait 50 days to test this
    timer0_millis = 4294962295UL;

    sei(); //enable interrupts again
}


// Print out the current time every second

void loop() {
    static unsigned long curTime;
    static unsigned long timeToFire;

    curTime = millis();

    if (curTime >= timeToFire) {
        Serial.print("Current time: "); Serial.println(millis());
        timeToFire = curTime + 1000;
    }
}
