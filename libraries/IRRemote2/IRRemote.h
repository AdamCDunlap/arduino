#ifndef IRRemote_h
#define IRRemote_h

#include "includes/IRButtonNameDeclarations.h"
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <pins_arduino.h>
#include <avr/pgmspace.h>

//"public" functions

// Call this function in setup() before pressing remote control buttons or calling irGetButton
void irInit(uint8_t interruptPinNum);


irButtonNames irGetButton(); // Returns an enum with the name of the name entered in the LabVIEW program
/* Recommended Usage:

switch(irGetButton()){
    case noButton: break;
    case enter: do_stuff(); break;
    case left: turn_robot_left(); break;
}

If you wish to store the enum in a variable, the type must be irButtonNames
Example:
irButtonNames button = irGetButton();

To print the name of the button, you can use the array irButtonNameStrings[]
Example:
irButtonNames button = irGetButton();
Serial.println(irButtonNameStrings[button]);
switch(irGetButton()){
    case noButton: break;
    case enter: do_stuff(); break;
}

If all you want to do is print the name, you can using irGetButtonName(), which returns a string.
This function simply calls irButtonNameStrings[irGetButton()], and has no efficiency gain over
 the previous method.
Example:
String buttonName = irGetButtonName();
Serial.println(buttonName);
*/
String irGetButtonName();



#endif //include guard
