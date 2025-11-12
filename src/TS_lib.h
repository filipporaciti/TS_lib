/*
*/
#ifndef TS_LIB_H
#define TS_LIB_H

#include "Arduino.h"

struct Page {
  const void* pointer;
  const size_t len;
};

class TS_lib
{
  public:
    static char* CODE_VERSION = "TSlib_11-2025";
    static char* PROTOCOL_VERSION = "002";

    TS_lib(const Stream* s1);
    void update();
    void setRealTimeStruct(const void* rt_data, const size_t structLen);
    void setPages(const Page* pages);
    void setCodeVersion(const char* code_version);
  private:
    Stream* _serial1;
};

#endif