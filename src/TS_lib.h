/*
*/
#ifndef TS_LIB_H
#define TS_LIB_H

#include <Arduino.h>
#include "communication.h"
#include "communication_legacy.h"
#include "pages.h"
#include "rt_data.h"

class TS_lib {
  public:
    static char* CODE_VERSION = "TSlib_11-2025";
    static char* PROTOCOL_VERSION = "002";

    TS_lib(const Stream* s1, const Rt_values* rt_data, const Page* pages);
    void update();
  private:
    const Stream* _serial1;
    const Communication _comm;
    const Communication_legacy _comm_legacy;
    const Pages _pages;
    const Rt_data _rt_data;

};

#endif