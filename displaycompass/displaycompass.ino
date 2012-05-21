#include <Wire.h>
#include <LEDArray.h>

LEDArray disp(A1, A0, A2);
void setup() {
    Wire.begin();
    Serial.begin(115200);
    disp.setPrintDuration(0);
    disp.setPrintType(FLASH);
    disp.setPrintBlock(BLOCK);
}

void loop() {
    Wire.beginTransmission(0x21);
    Wire.write('A');
    Wire.endTransmission();
    delay(6);
    Wire.requestFrom(0x21, 2);
    uint16_t reading = (Wire.read() << 8) | Wire.read();
         if (reading <  900) disp.write('N');
    else if (reading < 1800) disp.write('W');
    else if (reading < 2700) disp.write('S');
    else                     disp.write('E');
    Serial.println(reading);
}
