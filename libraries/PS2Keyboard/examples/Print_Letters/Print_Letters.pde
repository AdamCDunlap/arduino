#include <PS2Keyboard.h>

// Simple test program for new PS2Keyboard library
// Connect a PS2 keyboard to pins 3 & 4 (CLK and DATA respectively) and supply 5V to the keyboard
// For examples, see here: http://www.arduino.cc/playground/ComponentLib/Ps2mouse
// or                here: http://www.beyondlogic.org/keyboard/keybrd.htm
// That second article is a great place to start if you want to understand whats going on
//
// When you've compiled the code and uploaded it to the board, start a serial monitor at
// 9600bd.  Then press keys on your PS2 keyboard (the one connected to Arduino, not the one
// connected to your computer!) Try using <shift>, <ctrl> and <alt> keys
// and check that the caps_lock key sets the caps_lock light.
// Pressing <esc> key should reset the keyboard and you should see all 3 lights go on briefly.

#define KBD_CLK_PIN  3
#define KBD_DATA_PIN 4

PS2Keyboard keyboard;

void setup() {
  keyboard.begin(KBD_DATA_PIN);

  Serial.begin(9600);
  delay(1000);
}

#define is_printable(c) (!(c&0x80))   // don't print if top bit is set

void loop() {

  if(keyboard.available()) {

    // reading the "extra" bits is optional
    byte   extra = keyboard.read_extra(); // must read extra before reading the character byte
    byte       c = keyboard.read();

    boolean ctrl = extra & 1;  // <ctrl> is bit 0
    boolean  alt = extra & 2;  //  <alt> is bit 1

    if (ctrl) Serial.print('^');
    if (alt)  Serial.print('_');

    if      (c==PS2_KC_UP)      Serial.print("up\n");
    else if (c==PS2_KC_DOWN)    Serial.print("down\n");
    else if (c==PS2_KC_BKSP)    Serial.print("backspace\n");
    else if (c==PS2_KC_ESC)   { Serial.print("escape and reset\n"); keyboard.reset(); }
    else if ( is_printable(c) ) Serial.print(c);   // don't print any untrapped special characters
  } 
}
