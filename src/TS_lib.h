/*
*/
#ifndef TS_LIB_h
#define TS_LIB_h

#include "Arduino.h"

class TS_lib
{
  public:
    TS_lib(Stream* s1);
  private:
    Stream* _serial1;
};

#endif