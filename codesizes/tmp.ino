#include <STDINOUT.h>

//int serial_putchar(char c, FILE*) {
//    return !Serial.write(c);
//}

void setup() {
    //fdev_setup_stream(stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE);

    Serial.begin(115200);
    Serial.println("---");
    puts("hi there!");
    Serial.println("---");
    int i = 42;
    scanf("%d", &i);
    printf("You gave me %d/2\n", i*2);
}

void loop() {

}
