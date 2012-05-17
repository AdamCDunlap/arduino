#ifndef LEDArray_h
#define LEDArray_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <avr/pgmspace.h>

enum printTypes {FLASH, FADE, SCROLL};
enum printBlock_t {BLOCK, NOBLOCK};

template<uint8_t _size = 10>
class CircularBuffer {
public:
    CircularBuffer() : _in(0), _out(0) {}
    void put(unsigned char c) {
        _in = increment(_in);
        //If _in == _out (we've wrapped around), delete the last value
        if (_in == _out) _out = increment(_out);
        _buffer[_in] = c; //Write c to buffer
    }
    void del() {
        if (_in != _out) _in = decrement(_in);
    }
    unsigned char get() {
        //If there's nothing in the buffer, return 0
        if (_out == _in) return 0;
        //Else, increment _out and return value
        _out = increment(_out);
        return _buffer[_out];
    }
private:
    uint8_t _in, _out;
    char _buffer[_size];
    unsigned char increment(unsigned char c) {
        return (c == _size-1)? 0 : c+1;
    }
    unsigned char decrement(unsigned char c) {
        return (c==0)? _size-1 : c-1;
    }
};

class LEDArray : public Print {
	public:
		LEDArray (uint8_t _dataPin, uint8_t _clockPin, uint8_t _latchPin, uint8_t _numberOfMatricies = 1);
		~LEDArray();
		void drawPic (uint8_t* picture);
		void drawRow (uint8_t picture, uint8_t row);
		void setBrightness (uint8_t brightness);
		void shutdown();
		void wakeUp();
		void test();
		void stopTest();
		void clear();
        void allOn();
        void setPrintDuration(unsigned int time);
		void sendData(int data);
		void drawPicPmem (const unsigned char* picture);
        void setPrintType(printTypes type);
        void setPrintBlock(printBlock_t option);
#if ARDUINO >= 100
        size_t write(uint8_t);
#else
        void write(uint8_t);
#endif
        void backspace();
        void printRoutine();
//    virtual void write(const char *str);
	private:
        void scrollRow(uint8_t rowBuffer);
        unsigned int getDelayTime();
        bool charDone;
	    uint8_t dataPin;
	    uint8_t clockPin;
	    uint8_t latchPin;
        uint8_t dataBit;
        uint8_t clockBit;
        uint8_t latchBit;
        uint8_t dataBitNOT;
        uint8_t clockBitNOT;
        uint8_t latchBitNOT;
        volatile uint8_t *dataOut;
        volatile uint8_t *clockOut;
        volatile uint8_t *latchOut;
	    int numberOfMatricies;
        unsigned int printDelayTime;
        printTypes printType;
        bool blocking;
        uint8_t scrollBuffer[8];
        CircularBuffer<20> printBuffer;
        uint8_t currentBrightness;
};

class numDisplay {
	public:
		numDisplay (uint8_t _dataPin, uint8_t _clockPin, uint8_t _latchPin, uint8_t _digits);
        
		void shutdown();
		void wakeUp();
		void test();
		void stopTest();
		void setBrightness (uint8_t brightness);
		void sendData(int data, int chipSelect);
	private:
		uint8_t dataPin;
		uint8_t clockPin;
		uint8_t latchPin;
		int digits;
		int numberOfChips;
};
#endif
