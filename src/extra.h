#include <stdlib.h>

#ifndef _EXTRA_H
#define _EXTRA_H

#define MAP(V,MI,MO) ((float)V/MI) * MO

class IntTrip
{
  public:
    IntTrip(int8 a, int8 b,int8 c)
    {
      A = a;
      B = b;
      C = c;
    }
    IntTrip()
    {
      A = 0;
      B = 0;
      C = 0;
    }

    int8 A;
    int8 B;
    int8 C;
};
class FloatTrip
{
  public:
    FloatTrip(float a, float b, float c)
    {
      A = a;
      B = b;
      C = c;
    }
    FloatTrip()
    {
      A = 0;
      B = 0;
      C = 0;
    }

    float A;
    float B;
    float C;
};

#endif
