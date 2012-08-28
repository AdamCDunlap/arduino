#include <stdio.h>
#include <Arduino.h>
#include <STDINOUT.h>

// Function that printf and related will use to print
static int serial_putchar(char c, FILE* f) {
    if (c == '\n') serial_putchar('\r', f);
    return Serial.write(c) == 1? 0 : 1;
}
// Function that scanf and related will use to read
static int serial_getchar(FILE*) {
    // Wait until character is avilable
    while (Serial.available() <= 0);
    return Serial.read();
}

static FILE serial_stdinout;

//void setup_stdin_stdout() __attribute__ ((section (".init8")));

void setup_stdin_stdout() {
    
    // Set up stdout and stdin
    //fdev_setup_stream(stdout, serial_putchar, serial_getchar, _FDEV_SETUP_RW);
    //stdin = stderr = stdout;


    fdev_setup_stream(&serial_stdinout, serial_putchar, serial_getchar, _FDEV_SETUP_RW);
    stdout = &serial_stdinout;
    stdin  = &serial_stdinout;
    stderr = &serial_stdinout;
}

//struct blah {
//    blah(int ii) : i(ii) {
//        setup_stdin_stdout();
//    }
//    volatile int i;
//};
//
//static blah myblah(12);


size_t initializeSTDINOUT::initnum = 0;
initializeSTDINOUT::initializeSTDINOUT() {
    if (initnum++ == 0) {
        setup_stdin_stdout();
    }
}

