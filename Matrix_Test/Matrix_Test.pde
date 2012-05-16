#include <avr/interrupt.h>
#include <LEDArray.h>

unsigned long calltimes = 0;
bool isrCalled = false;

#define RESOLUTION 65536

//ISR(TIMER1_OVF_vect) {
//  calltimes++;  
//  isrCalled = true;
//}

void setPeriod(long microseconds)
{
  uint8_t clockSelectBits;
  long cycles = (F_CPU / 2000000) * microseconds;                                // the counter runs backwards after TOP, interrupt is at BOTTOM so divide microseconds by 2
  Serial.println(cycles);
  if(cycles < RESOLUTION)              clockSelectBits = _BV(CS10);              // no prescale, full xtal
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11);              // prescale by /8
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11) | _BV(CS10);  // prescale by /64
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12);              // prescale by /256
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12) | _BV(CS10);  // prescale by /1024
  else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS12) | _BV(CS10);  // request was out of bounds, set as maximum
  ICR1 = cycles;                                                                 // ICR1 is TOP in p & f correct pwm mode
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
  TCCR1B |= clockSelectBits;                                                     // reset clock select register
}

LEDArray disp(9,8,10);

void setup() {
  Serial.begin(9600);
  Serial.println("starting");
  //Serial.println(TCCR1A, BIN);
  //Serial.println(TCCR1B, BIN);
  //TCCR1A = B00000000;                 // clear control register A 
  //TCCR1B = _BV(WGM13);        // set mode 8: phase and frequency correct pwm, stop the timer
  //setPeriod(1000000);
  //TIMSK1 = _BV(TOIE1);
  disp.setPrintDuration(1000);
  disp.setPrintType(SCROLL);
  disp.setPrintBlock(NOBLOCK);
}

void loop() {
  static unsigned long n = 0;
  disp.print("loa;");
  Serial.println("eh");
  delay(5000);
}
