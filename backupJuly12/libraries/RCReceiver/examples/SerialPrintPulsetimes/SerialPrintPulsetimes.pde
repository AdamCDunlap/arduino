#include <RCReceiver.h>                 // Include the library

void setup(){
  Serial.begin(9600);                   // Begin Serial communication at 9600 baud
  RCReceiverBegin(2, 3);                // Set up the interrupts to get the pulse lengths
}

void loop(){
  for (int i=0;i<4;i++){                // Repeat 4 times, one for each channel
    Serial.print(pulseTime(i, 100000)); // Read each channel, discarding old values after 100000ms
    Serial.print("\t");                 // Print a tab
  }
  Serial.println();                     // Print a newline
  delay(50);                            // Don't overload the serial line
}
