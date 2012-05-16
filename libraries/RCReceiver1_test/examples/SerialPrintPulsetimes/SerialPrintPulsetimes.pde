#include <PinChangeInt.h>
#include <TimerOne.h>
#include <RCReceiver1.h>       // Include the libraries

unsigned int time;

void setup(){
  Serial.begin(9600);          // Begin Serial communication at 9600 baud
  RCReceiverBegin(2, 3);       // Set up the interrupts to get the pulse lengths
  Serial.println("Beginning"); // Print a starting message
}

void loop(){
  for (int i=0;i<4;i++){       // Repeat 4 times, one for each channel
    time = pulseTime(i, 1000); // Read each channel, discarding old values after 1 second
    Serial.print(time);        // Print the pulsetime
    Serial.print("\t");        // Print a tab
  }
  Serial.println();            // Print a newline
  delay(250);                  // Don't overload the serial line
}
