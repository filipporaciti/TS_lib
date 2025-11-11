/*
*/
#ifndef TS_LIB_h
#define TS_LIB_h

#include "Arduino.h"

struct Page {
  const void* pointer;
  const size_t len;
};

class TS_lib
{
  public:
    TS_lib(const Stream* s1);
    void update();
    void setRealTimeStruct(const void* rt_data, const size_t structLen);
    void setPages(const Page* pages);
  private:
    Stream* _serial1;
};

#endif