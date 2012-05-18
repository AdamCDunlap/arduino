/* move_with_serial.ino
 * Receives commands from a serial line and moves the arduinobot accordingly
 *
 * Syntax:
 *   <'r',x,y,z> or <'m',m1,m2,m3>
 *   First value is r or m, for Robot or Motors. If it's r, move the robot
 *     according to the x, y, and z vectors given
 *   If it's m, move each motor according to the speeds given
 *   If the syntax is incorrect, don't do anything
 *  
 */

//#define COMPUTER_INTERFACE
//#define NOPRINT
#include "syntax.h"
#include <CircleBot.h>
#include <AFMotor.h>

#ifdef COMPUTER_INTERFACE
#define NOPRINT
#endif

CircleBot bot(1, 3, 4);

void setup() {
    Serial.begin(115200);
    #ifndef NOPRINT
//    Serial.print(F(
//    "Welcome to the Ardubot Serial Interface.\n"
//    "The Syntax is:\n"
//    "<'r',x,y,z> or <'m',m1,m2,m3>\n"
//    "First value is r or m, for Robot or Motors. If it's r, move the robot\n"
//    "  according to the x, y, and z vectors given\n"
//    "If it's m, move each motor according to the speeds given\n"
//    "If the syntax is incorrect, don't do anything\n"
//    ));
      Serial.println("hi there!");
    #endif
}

const size_t maxCmdLen = 20;
char command[maxPacketLen];

void loop() {
    static size_t cmdlen = 0;
    static bool cmdstarted = false;
    static bool cmdended = false;

    while (Serial.available()) {
        char inchar = Serial.read();

        if (inchar == packetStart) {
            cmdstarted = true;
            cmdended = false;
            cmdlen = 0;
        }
        else if (inchar == packetEnd) {
            cmdended = true;
            command[cmdlen] = '\0'; // NULL terminate
            break; // Break out of loop so we'll be sure to process even
                   //   if there's another char available
        }
        else if (cmdlen < maxPacketLen && cmdstarted && !cmdended) { 
            command[cmdlen] = inchar;
            cmdlen++;
        }
    }
    if (cmdstarted && cmdended) { // a full packet has been received

        
        const char* delims = ",";
        char* splitcmd[maxArgListLen];
        for (size_t i=0; i<maxArgListLen; i++) {
            splitcmd[i] = NULL;
        }

        // Skip the control character and first comma
        splitcmd[0] = strtok(command+2, delims); 
        size_t numcmds=0;
        while (splitcmd[numcmds]) {
            splitcmd[++numcmds] = strtok(NULL, delims);
        } 

        switch (command[0]) {
        case rbtStx::cmd:
            if (numcmds != rbtStx::length) {
                #ifndef NOPRINT
                Serial.println("Wrong number of arguments");
                #endif
                break;
            }
            { // Make a new block so we can define local variables
                int x = constrain(atoi(splitcmd[rbtStx::xPos]), -1000, 1000);
                int y = constrain(atoi(splitcmd[rbtStx::yPos]), -1000, 1000);
                int z = constrain(atoi(splitcmd[rbtStx::zPos]), -1000, 1000);
                bot.Move(x, y, z);
                #ifndef NOPRINT
                Serial.print("Moving the robot with");
                Serial.print("\tx: "); Serial.print(x);
                Serial.print("\ty: "); Serial.print(y);
                Serial.print("\tz: "); Serial.print(z);
                Serial.println();
                #endif
            }
            break;
        case mtrStx::cmd:
            if (numcmds != mtrStx::length) {
                #ifndef NOPRINT
                Serial.println("Wrong number of arguments");
                #endif
                break;
            }
            { // Make a new block so we can define local variables
                int mtr0spd = constrain(atoi(splitcmd[mtrStx::mtr0Pos]), -1000, 1000);
                int mtr1spd = constrain(atoi(splitcmd[mtrStx::mtr1Pos]), -1000, 1000);
                int mtr2spd = constrain(atoi(splitcmd[mtrStx::mtr2Pos]), -1000, 1000);
                bot.RunMotors(mtr0spd, mtr1spd, mtr2spd);
                #ifndef NOPRINT
                Serial.print("Moving motors with speeds");
                Serial.print("\t0: "); Serial.print(mtr0spd);
                Serial.print("\t1: "); Serial.print(mtr1spd);
                Serial.print("\t2: "); Serial.print(mtr2spd);
                Serial.println();
                #endif
            }
            break;
        case rawStx::cmd:
            if (numcmds != rawStx::length) {
                #ifndef NOPRINT
                Serial.println("Wrong number of arguments");
                #endif
                break;
            }
            { // Make a new block so we can define local variables
                int mtr0spd = constrain(atoi(splitcmd[rawStx::mtr0Pos]), -255, 255);
                int mtr1spd = constrain(atoi(splitcmd[rawStx::mtr1Pos]), -255, 255);
                int mtr2spd = constrain(atoi(splitcmd[rawStx::mtr2Pos]), -255, 255);
                bot.RunRawMotors(mtr0spd, mtr1spd, mtr2spd);
                #ifndef NOPRINT
                Serial.print("Moving motors with actual speeds:");
                Serial.print("\t0: "); Serial.print(mtr0spd);
                Serial.print("\t1: "); Serial.print(mtr1spd);
                Serial.print("\t2: "); Serial.print(mtr2spd);
                Serial.println();
                #endif
            }
            break;
        default:
            #ifndef NOPRINT
            Serial.println("Bad command");
            #endif
            break; // Bad syntax
        }

        cmdstarted = cmdended = false;
    }
}
