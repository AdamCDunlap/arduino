#ifndef RunningPercentile_h
#define RunningPercentile_h

#include <inttypes.h>

#define RUNNINGPERCENTILEVERSION "1.0"
// should at least be 5 and odd.
//#define Percentile_SIZE 5

class RunningPercentile 
{
	public:
	RunningPercentile(int, int);
	~RunningPercentile();
	void clear();
	void add(int);
	int getPercentile();

protected:
	uint8_t _cnt;
	uint8_t _idx;
	int *_ar;
	int *_as;
	void sort();
	uint8_t _which;
	uint8_t _size;
};

#endif
// END OF FILE