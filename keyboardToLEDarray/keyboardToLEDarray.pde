#include <PS2Keyboard.h>
#include <LEDArray.h>

const int dataPin = A5;

PS2Keyboard keyboard;
LEDArray disp(9,8,10);

void setup() {
  Serial.begin(9600);
  keyboard.begin(dataPin);
  disp.setPrintDuration(400);
  disp.setPrintType(SCROLL);
  Serial.println("If you want to print via Serial Monitor,"
  "keep it under 10 characters at a time or the buffer will run out.\n"
  "Hola, my name is Adam");
  disp.println("Hola, my name is Adam!"
  //"Well not really, I'm just an arduino,"
  //"but my favorite human in the world is named Adam!"
  );
  disp.setPrintBlock(NOBLOCK);
}

#define is_printable(c) (!(c&0x80))   // don't print if top bit is set

void loop() {

  if(keyboard.available()) {

    // reading the "extra" bits is optional
    byte   extra = keyboard.read_extra(); // must read extra before reading the character byte
    byte       c = keyboard.read();

    boolean ctrl = extra & 1;  // <ctrl> is bit 0
    boolean  alt = extra & 2;  //  <alt> is bit 1

    if (ctrl) disp.write('^');
    if (alt)  disp.write('_');

    if (c==PS2_KC_ESC)   keyboard.reset();
    else if (c == PS2_KC_BKSP) disp.backspace();
    else if ( is_printable(c) ) {disp.write(c);}   // don't print any untrapped special characters
    Serial.write(c);
  }
  if (Serial.available()) {
    char c = Serial.read();
    disp.write(c);
    Serial.write(c);
  }
}
