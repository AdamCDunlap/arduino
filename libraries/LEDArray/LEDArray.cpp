#include <LEDArray.h>
#include <pins_arduino.h>
#include "includes/characters.h"

namespace LEDArray_ns {
    LEDArray *curClass = 0; //Stores the instance of the class that theinterrupt should call to
}

ISR(TIMER1_OVF_vect) {
    sei(); //This interrupt is quite long, so enable nested interrupts
    LEDArray_ns::curClass -> printRoutine(); //Calls printRoutine() from the instance of the class that is in LEDArray_ns::curClass
}

LEDArray::LEDArray(uint8_t _dataPin, uint8_t _clockPin, 
	               uint8_t _latchPin, uint8_t _numberOfMatricies) : printBuffer() {
    dataPin = _dataPin;
    clockPin = _clockPin;
    latchPin = _latchPin;
    numberOfMatricies = _numberOfMatricies;
    dataBit = digitalPinToBitMask(dataPin);  // Adapted from digitalWrite
    clockBit = digitalPinToBitMask(clockPin);
    latchBit = digitalPinToBitMask(latchPin);
    dataBitNOT = ~dataBit;
    clockBitNOT = ~clockBit;
    latchBitNOT = ~latchBit;
    uint8_t dataPort = digitalPinToPort(dataPin);
    uint8_t clockPort = digitalPinToPort(clockPin);
    uint8_t latchPort = digitalPinToPort(latchPin);
    dataOut = portOutputRegister(dataPort);
    clockOut = portOutputRegister(clockPort);
    latchOut = portOutputRegister(latchPort);

    #if defined(UCSRB) //Change the Serial pins back to normal pins
        UCSRB = 0;
    #elif defined(UCSR0B)
        UCSR0B = 0;
    #endif
    
    initialize();
 }

void LEDArray::initialize() {

    charDone = true;

    for(uint8_t i=0;i<8;i++){
        scrollBuffer[i] = 0;
    }

	pinMode(dataPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(latchPin, OUTPUT);
	digitalWrite(dataPin, LOW);
	digitalWrite(clockPin, LOW);
	digitalWrite(latchPin, LOW);

    digitalWrite(connectedPin, HIGH); // Enable pullup
    wasDispConnected = true;

    sendData(0x00); //Try to clear the line so that only good data gets through
//	for(uint8_t i=0; i<numberOfMatricies;i++){
		sendData(0x0900); //Decode Mode: None
		sendData(0x0A0F); //Intensity: 31/32 (Max)
		sendData(0x0B07); //Scan Limit: 0-7 (All)
//	}
    setPrintDuration(1000); //set default print duration to be 1000ms
    setPrintType(SCROLL);
    setPrintBlock(BLOCK);
	clear();
	wakeUp();
}

LEDArray::~LEDArray(){
	pinMode(dataPin, INPUT);
	pinMode(clockPin, INPUT);
	pinMode(latchPin, INPUT);
    digitalWrite(connectedPin, LOW);
}

void LEDArray::sendData(int data){

  if (isConnected()) return;

  uint8_t oldSREG;
  for (int i=0;i<16;i++) {
    if (data & (_BV(15-i))){ //If the (15-i)th bit of data is 1:
      oldSREG = SREG;
      cli();                 //disables interrupts
      *dataOut |= dataBit;   //digitalWrite(dataPin, HIGH);
    }
    else{
      oldSREG = SREG;
      cli();                 //disable interrupts
      *dataOut &= dataBitNOT;//digitalWrite(dataPin, LOW);
    }
    *clockOut |= clockBit;   //digitalWrite(clockPin, HIGH);
    *clockOut &= clockBitNOT;//digitalWrite(clockPin, LOW);
    SREG = oldSREG;          //enable interrupts
  }
  oldSREG = SREG;
  cli();                     //disable interrupts
  *latchOut |= latchBit;     //digitalWrite(latchPin, HIGH);
  *latchOut &= latchBitNOT;  //digitalWrite(latchPin, LOW);
  *dataOut &= dataBitNOT;    //digitalWrite(dataPin, LOW);
  SREG = oldSREG;            //Restore interrupts
}

bool LEDArray::isConnected() {
  if (connectedPin < 0) return true; // Assume it's connected if there's no pin
  bool ret = !digitalRead(connectedPin);
  if (ret && !wasDispConnected) initialize();
  wasDispConnected = ret;
  return ret;
}

void LEDArray::setBrightness(uint8_t brightness){
  brightness /= 6.7; //convert from 0-100 to 0-15
  if (brightness >= 15){
    sendData(0x0A0F);
  }
  else {
	  sendData(0x0A00|brightness); //brightness is one nibble, so when OR'd it will go in last digit
  }
  currentBrightness = brightness; //set the instance variable flag
}

void LEDArray::shutdown(){
	sendData(0x0C00);
}

void LEDArray::wakeUp(){
	sendData(0x0C01);
}

void LEDArray::test(){
	sendData(0x0F01);
}

void LEDArray::stopTest(){
	sendData(0x0F00);
}

void LEDArray::clear(){
	for (uint8_t i=0;i<8;i++){
		sendData((i+1)<<8); //have nothing in the last byte
	}
}

void LEDArray::allOn(){
  for (uint8_t i=0;i<8;i++) {
    sendData(((i+1)<<8)|0xFF); //Turn the entire row on
  }
}

void LEDArray::drawPic(uint8_t* picture){
	for(uint8_t i=0;i<8;i++){
		sendData((i+1)<<8 | picture[i]);
	}
}

void LEDArray::drawRow(uint8_t picture, uint8_t row) {
	sendData(((row + 1) << 8) | picture);
}

void LEDArray::drawPicPmem(const unsigned char* pictureInMemory){
	uint8_t picture[8] = {0,0,0,0,0,0,0,0};
	for(uint8_t i=0;i<8;i++){
		picture[i] = pgm_read_byte_near(&pictureInMemory[i]); //read one byte (one row) from program memory
	}
	drawPic(picture);
}

void LEDArray::setPrintDuration(unsigned int time){
  printDelayTime = time; 
}

void LEDArray::setPrintType(printTypes type){
  printType = type;
}

void LEDArray::setPrintBlock(printBlock_t option){
    if (option == BLOCK) {
        blocking = true;
        LEDArray_ns::curClass = 0;

        // The following lines SHOULD let analogWrite work on Timer1 again.
        #if defined TIMSK1 && defined TCCR1A &&\
            defined WGM10 && defined CS10 && defined CS11
        TIMSK1 = 0;
        TCCR1A = WGM10;
        TCCR1A = CS10 | CS11;
        #endif
    }
    #if defined TCCR1A && defined TCCR1B && defined CS10 && defined CS11 && defined CS12 && defined ICR1
    else if (option == NOBLOCK) {
        TCCR1A = 0x00;
        TCCR1B = _BV(WGM13);
        long cycles = (long)(F_CPU / 2000) *  getDelayTime();
        //Following taken from TimerOne library 
        if(cycles < 65536)              TCCR1B |= _BV(CS10);              // no prescale, full xtal
        else if((cycles >>= 3) < 65536) TCCR1B |= _BV(CS11);              // prescale by /8
        else if((cycles >>= 3) < 65536) TCCR1B |= _BV(CS11) | _BV(CS10);  // prescale by /64
        else if((cycles >>= 2) < 65536) TCCR1B |= _BV(CS12);              // prescale by /256
        else if((cycles >>= 2) < 65536) TCCR1B |= _BV(CS12) | _BV(CS10);  // prescale by /1024
        else        cycles = 65536 - 1, TCCR1B |= _BV(CS12) | _BV(CS10);  // request was out of bounds, set as maximum
        ICR1 = cycles;                                                    // ICR1 is TOP in p & f correct pwm mode
        blocking = false;
        
        LEDArray_ns::curClass = this; //Set which class's interrupt to call
        sei(); // Ensure that interrupts are globally enabled
        TIMSK1 = _BV(TOIE1); //Enable the interrupt
    }
    #endif //TCCR1A, etc
}

inline unsigned int LEDArray::getDelayTime() {
    switch (printType) {
        case FLASH:  return printDelayTime;
        case FADE:   return printDelayTime / 30;
        case SCROLL: return printDelayTime / (bytesPerChar + 1);
        default: return 0; 
    }
}

size_t LEDArray::write(uint8_t character) {
    printBuffer.put(character);
    if (blocking) {
        do {
            printRoutine();
            delay(getDelayTime());
        } while (!charDone);
    }
    else {
        #if defined TIMSK1 && defined TOIE1
        TIMSK1 = _BV(TOIE1); //Reenable the interrupt
        #endif
    }
    return 1;
}  

void LEDArray::printRoutine() {
    static unsigned char character;
    if (charDone) character = printBuffer.get(); 
    if (character) { //Don't do anything if there isn't a char waiting
                      //printBuffer.get() returns 0 if there's nothing there

        if (printType == FLASH) {
	        unsigned char picture[8] = {0,0,0,0,0,0,0,0};
            unsigned char j=0;

	        for(uint8_t i=(8 - bytesPerChar)/2;i<(8 + bytesPerChar)/2;i++){
		        picture[i] = pgm_read_byte_near(&matrixChars[character][j]); //read one byte (one row) from program memory
                j++;
	        }
        
	        drawPic(picture);
            charDone = true;
        }
  
        else if (printType == SCROLL) {
            //if(!charDone) {
                static uint8_t i = 0;
                uint8_t rowBuffer;
                if (i >= bytesPerChar) {
                    scrollRow(0);
                    charDone = true;
                    i = 0;
                }
                else {
                    charDone = false;
                    do {
                        #if defined REVERSED_CHARACTERS
                        rowBuffer = pgm_read_byte_near(&(matrixChars[character][(bytesPerChar-1)-i])); //read one byte (one row) from program memory in reverse order
                        #else //REVERSED_CHARACTERS
                        rowBuffer = pgm_read_byte_near(&(matrixChars[character][i])); //read bytes
                        #endif //REVERSED_CHARACTERS
                        i++;
                        if (i >= bytesPerChar){
                            if (rowBuffer == 0) {
                                scrollRow(0);
                                charDone = true;
                                i = 0;
                                return; // Don't print last row if it's blank
                            }
                            break;
                        }
                    } while (rowBuffer == 0 && (character != 32 || i <= 2) && character != 10 && character != 13); //delete blank lines except on space, newline, and carriage return
            
                    scrollRow(rowBuffer);
                }
            //}
        }
    
        else if (printType == FADE) {
	        uint8_t picture[8] = {0,0,0,0,0,0,0,0};
            static uint8_t brightness = 0;
            static bool direction = true; //true=up,false=down
            if (charDone) {
                charDone = false;
                uint8_t j = 0;
	            for(uint8_t i=(8 - bytesPerChar)/2;i<(8 + bytesPerChar)/2;i++){
		            picture[i] = pgm_read_byte_near(&matrixChars[character][j]); //read one byte (one row) from progmem
                    j++;
	            }
                drawPic(picture);
            }
            if (direction) {
                sendData(0x0A00|brightness++);
                if (brightness > 14) direction = false;
            }
            else /*if (brightness > 0)*/ {
                sendData(0x0A00|brightness--);
                if (brightness < 1) direction = true, charDone = true;
            }
            #if 0
            else { //After entire up-down loop is done
                //clear(); // Get rid of character
                sendData(0x0AA|currentBrightness); //Restore original brightness
                direction = true;
                charDone = true;
            }
            #endif
        } 
    }
    else{ //No new character
        charDone = true; //End the print loop
        #if defined TIMSK1
        if (!blocking) {
        TIMSK1 = 0; // Disable the timer1 interrupt until write is called again
        }
        #endif
    }
}

void LEDArray::scrollRow(uint8_t newRow) {
    #if defined REVERSED_CHARACTERS
    for (uint8_t i=7;i>0;i--){
        scrollBuffer[i] = scrollBuffer[i-1];
    }
    scrollBuffer[0] = newRow;

    #else //REVERSED_CHARACTERS

    for (uint8_t i=0;i<7;i++){
        scrollBuffer[i] = scrollBuffer[i+1];
    }
    scrollBuffer[7] = newRow;
    #endif //REVERSED_CHARACTERS

    drawPic(scrollBuffer);
}

void LEDArray::backspace() {
    printBuffer.del();
}

#if 0
void LEDArray::write(const char *str){
  
  if (printType == SCROLL){
    unsigned int delayTime = printDelayTime/8; //there are 8 "shifts" until the new letter
    #if 0
    uint8_t picture[8] = {0,0,0,0,0,0,0,0};
    for(uint8_t i=0;i<5;i++){
      picture[i+3] = pgm_read_byte_near(&(matrixChars[*str][i])); //read one byte (one row) from program memory
	  }
    drawPic(picture);
    str++;
    delay(delayTime);
    #endif
    while (*str) { //Keep going until we get to the NULL character
      for(uint8_t i=0;i<5;i++){
        for(uint8_t j=0;j<7;j++){
          scrollBuffer[j] = scrollBuffer[j+1];
        }
        scrollBuffer[7] = pgm_read_byte_near(&(matrixChars[*str][i])); //read one byte (one row) from program memory
        drawPic(scrollBuffer);
        delay(delayTime);
      }
      str++;
    } 
  }

  else { //if we're not SCROLLing, do one character at a time
    while (*str)
      write(*str++);
  }
}
#endif

//void numDisplay::sendData(int data, int chipSelect = 0){
//	for(int i=0;i<(chipSelect >> 3);i++){
//		digitalWrite(clockPin, HIGH); //Give zeroes to all
//		digitalWrite(clockPin, LOW); //the chips before
//	}	
//	shiftOut(dataPin, clockPin, MSBFIRST, data>>8);
//	shiftOut(dataPin, clockPin, MSBFIRST, data);
//	digitalWrite(latchPin, HIGH);
//	digitalWrite(latchPin, LOW);
//	digitalWrite(dataPin, LOW);
//}
//
//numDisplay::numDisplay (uint8_t _dataPin, uint8_t _clockPin,
//	uint8_t _latchPin, uint8_t _digits = 1){	
//	dataPin = _dataPin;
//	clockPin = _clockPin;
//	latchPin = _latchPin;
//	digits = _digits;
//	pinMode(dataPin, OUTPUT);
//	pinMode(clockPin, OUTPUT);
//	pinMode(latchPin, OUTPUT);
//	digitalWrite(dataPin, LOW);
//	digitalWrite(clockPin, LOW);
//	digitalWrite(latchPin, LOW);
//	numberOfChips = (digits >> 3) + 1; //8 digits per chip
//	for(uint8_t i=0; i<numberOfChips;i++){
//		sendData(0x09FF, i); //Decode Mode: All
//		sendData(0x0A0F, i); //Intensity: 31/32 (Max)
//	}
//	if(numberOfChips == 1){
//		sendData(0x0B00 | digits); //Scan Limit: Digits 
//	}
//	else {
//		for(int i=0;i<numberOfChips;i++){ // If we have more than one chip,
//			sendData(0x0B07, i);          // set the scan limit to max on each one
//		}                                 // so the brightnesses are equal.
//	}
//}
