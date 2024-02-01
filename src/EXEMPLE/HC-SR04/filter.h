#ifndef filter_h
#define filter_h

#include "MedianFilter.h"

class Kalman
{
public:
   Kalman();
   ~Kalman();
   int filter(int U);

private:
   static const int R;
   static const int H;
   static int Q;
   static int P;
};
int moving_average_filter(int *vector, int size);
long filtered_ultradist(long time, long distance);

#endif
