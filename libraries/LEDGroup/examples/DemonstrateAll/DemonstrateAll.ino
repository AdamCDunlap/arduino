// DemonstrateAll.ino
#include <LEDGroup.h>

int group1pins[] = {2, 4, 5, 7};
LEDGroup group1(group1pins, 4);

void setup() {
}

void loop() {
    group1.runPattern(Patt::CIRCULAR, 250);
}
