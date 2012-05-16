/* This sketch demonsrates every function currently implimented in the LEDArray library.
 * The LEDArray library is meant to be used with the MAX7221/MAX7219 chips and an 8x8, single color LED matrix
 */

#include <LEDArray.h>

/* Functions:
 * drawPic(byte[8] picture) - Draws the array that is given - each 1 bit turns the corresponding LED on.
 * drawPicPmem (prog_uchar[8] picture) - Draws the array in program memory - give it the name of the array
 * drawRow(byte picture, byte row) - Draws the given row with the bits of "picture"
 * all "print" or "write" functions, same as in Serial - Prints out letters and words on the screen, one at a time. Font is located in LEDArray/includes/characters.h
 * setPrintType(FLASH SCROLL or FADE) - Sets how the print functions work - FLASH displays each letter, then moves on to the next one, FADE continuously adjusts the brightness up and then down for each letter, and SCROLL scrolls each letter across the screen
 * setPrintDuration(unsigned int time) - Sets how long each letter is displayed with the print functions
 * setBrightness(byte brightness) - Sets the brightness of each LED - brightness is 0-100; there are 15 different values
 * shutdown() - Turns off all the LEDs; can accept data but won't turn on LEDs until wakeUp() is called
 * wakeUp() - Turns off shutdown mode
 * test() - Turns on all LEDs
 * stopTest() - Turns off test
 * clear() - Turns off all the LEDs; can be turned on by drawPic, drawPicPmem, drawRow, or the print functions
 * allOn() - Turns on all the LEDs; can still be modified real-time by other functions
 * sendData(int data) - **ADVANCED FUNCTION** Sends two bytes of data to the MAX chip - please see datasheet
 */

LEDArray disp(9, 10, 11);
/* Set up the array with:
 * DIN on arduino pin 9 (DIN is pin 1 of the MAX72**)
 * CLK on arduino pin 10 (CLK is pin 13 of the MAX72**)
 * LOAD/CS on arduino pin 11 (LOAD is pin 12 of the MAX7219, CS is pin 12 of the MAX7221)
 * 
 * Attach cathodes (negatives) of LED Array to pins 2, 11, 6, 7, 3, 10, 5, 8 of the MAX72** chip (in order)
 * Attach anodes (positives) of LED Array to pins 14, 16, 20, 23, 21, 15, 17, 22 of the MAX72** chip (in order)
 * Attach 5V to pin 19 of the MAX72**
 * Attach GND to pins 4 and 9 of the MAX72**
 * Attach a resistor from pin 18 of the MAX72** to 5V with a value determioned on page 11 of the MAX7219/MAX7221 datasheet, in table 11
 */



void setup()
{
  /*Have nothing to do!*/
}

unsigned char smiley[8] = {               // Define a picture of a smiley face
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100,
};

prog_uchar heart[8] = {          // Define a picture of a heart in program memory, so it does not take up valuable RAM
  B01100110,
  B11011011,
  B11000011,
  B11000011,
  B11100111,
  B01100110,
  B00111100,
  B00011000,
};

void loop()                      // Run this forever
{
  disp.test();                   // Turn on all LEDs, full brightness
  delay(500);
  disp.stopTest();               // Turn the LEDs back off
  delay(500);
  
  disp.allOn();                  // Turn on all the LEDs
  delay(500);                    // Wait 1/2 second
  
  disp.drawPic(smiley);          // Draw the smiley face on the screen

  for (int i = 0; i <= 100; i++) // Run a loop 101 times, to get values from 0 to 100
  {
    disp.setBrightness(i);       // Set the brightness of the display to the current value of the loop
    delay (10);                  // Wait 10 milliseconds between each value
  }
  for (int i = 15; i >= 0; i--)  // Run a loop 16 times, with values from 15 to 0
  {
    disp.sendData(0x0A00 | i);   // Set the brightness of the display to the current value of the loop by sending the data directly
    delay (10);                  // Wait 10 milliseconds between each value
  }

  disp.setBrightness(100);       // Set brigthness back to maximum

  disp.shutdown();               // Turn off all LEDs - The MAX72** chip can still receive commands!

  disp.drawPicPmem(heart);       // Get ready to draw the heart picture from program memory - Display is still in shutdown mode!
  delay(500);                    // Keep display off for 1/2 second

  disp.wakeUp();                 // Turn shutdown mode off and show the picture of the heart
  delay(1000);                   // Delay one second so picture can be seen
  
  disp.clear();                  // Shut off all LEDs and clear the chip's memory
  delay(500);                    // Keep off for 1/2 second
  
  disp.drawRow(B11111111, 0);    // Turn the first row completely on
  disp.drawRow(B00000000, 1);    // Turn the second row completely off
  disp.drawRow(B10101010, 2);    // Turn the third row alternating on
  disp.drawRow(B01010101, 3);    // Turn the fourth row alternating on
  disp.drawRow(B00001111, 4);    // Turn the fifth row half on
  disp.drawRow(B11110000, 5);    // Turn the sixth row half on
  disp.drawRow(B11001100, 6);    // Turn the seventh row double-alternating on
  disp.drawRow(B00110011, 7);    // Turn the eighth and final row double-alternating on
  delay(2000);                   // Wait 2 seconds to admire the display
  
  disp.setPrintType(FLASH);      // Set the print type to FLASH - Show one character after another with no transistion
  disp.print("Flash!");          // Print the word "Flash!" to the display
  
  disp.setPrintDuration(2000);   // Set the print function to print one character every two seconds - slow!
  disp.setPrintType(FADE);       // Set the print type to FADE - Fade into and out of each character
  disp.print("Fade");            // Print the word "Fade" to the display
  
  disp.setPrintDuration(500);    // Set the print function to print two characters per second - fast!
  disp.setPrintType(SCROLL);     // Set the print type to SCROLL - Scroll the characters across the screen
  disp.println("SCROLL...");     // Print the word "SCROLL..." to the display
  
  disp.print(3.1415926);         // Print part of pi to the display
  disp.write(147);               // Print the ASCII code 147 to the display - o with circumflex, but font may be different
  delay(500);
} 




