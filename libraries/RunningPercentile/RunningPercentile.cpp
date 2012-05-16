#include "RunningPercentile.h"
#include <stdlib.h>

RunningPercentile::RunningPercentile(int which, int size)
{
	_size = size;
	_which = which;
	_ar = (int*)calloc(size,sizeof(int));
	_as = (int*)calloc(size,sizeof(int));
	clear();
}

RunningPercentile::~RunningPercentile()
{
	free(_ar);
	free(_as);
}

// resets all counters
void RunningPercentile::clear()
{ 
	_cnt = 0;
	_idx = 0;
}

// adds a new value to the data-set
// or overwrites the oldest if full.
void RunningPercentile::add(int value)
{
	_ar[_idx++] = value;
	if ( _idx >= _size ) _idx = 0; // wrap around
	if (_cnt < _size) _cnt++;
	return;
}

int RunningPercentile::getPercentile()
{
	sort();
	return _as[_cnt*_which/_size];
}


void RunningPercentile::sort()
{
	// copy
	for (uint8_t i=0; i< _cnt; i++) _as[i] = _ar[i];

	// sort all
	for (uint8_t i=0; i< _cnt-1; i++) 
	{
		uint8_t m = i;
		for (uint8_t j=i+1; j< _cnt; j++)
		{
			if (_as[j] < _as[m]) m = j;
		}
		if (m != i)
		{
			long t = _as[m];
			_as[m] = _as[i];
			_as[i] = t;
		}
	}
}

// END OF FILE