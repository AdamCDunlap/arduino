/* Written so that user can call one method to update every instance of this class.
 * Don't need because for SoftBlink, user will call equivelent of digitalWrite  
 *  for each instance anyway, and that can update
 */


#ifndef SOFTBLINK_H
#define SOFTBLINK_H

#include <Arduino.h>

class SoftBlink {

public:
// "Constructor"
    static SoftBlink* NewLED(int inpinNum, unsigned int infadeTime = 100) {
        numBlinkers++;
        if (Blinkers_size < numBlinkers) {
            // Store pointer to old array, make new array, copy elements from
            //  old (tmp) to new (Blinkers), delete old array
            SoftBlink** tmp = Blinkers;
            Blinkers = new SoftBlink*[Blinkers_size *= 2];
            for (int i=0; i<numBlinkers-1; i++) {
                Blinkers[i] = tmp[i];
            }
            delete[] tmp;
        }
        Blinkers[numBlinkers] = new SoftBlink(inpinNum, infadeTime);
    }

// "Destructor"
    static void DeleteLED(SoftBlink* Blinker) {
        int i;
        for (i=0; i<numBlinkers; i++) {
            if (Blinker == Blinkers[i]) {
                delete Blinkers[i];
            }
        }
        // Shift others down
        for (i++; i<numBlinkers; i++) {
            Blinkers[i-1] = Blinkers[i];
        }
        numBlinkers--;
    }
    static void update() {
        for (int i=0; i<numBlinkers; i++) {
            Blinkers[i]->update_single();
        }
    }

private:
    // Private constructor and destructor
    SoftBlink(int inpinNum, unsigned int infadeTime) : pinNum(inpinNum), fadeTime(infadeTime) {

    }

    ~SoftBlink();

    void update_single() {
        
    }

    // Array of pointers to blinkers
    static SoftBlink** Blinkers;
    static uint8_t numBlinkers; // SET = 0
    static uint8_t Blinkers_size; // SET = 0

    int pinNum;
    unsigned int fadeTime;
    bool lastState;
    unsigned long changeTime;
}

#endif
