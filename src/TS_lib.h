/*
*/
#ifndef TS_LIB_H
#define TS_LIB_H

#include <Arduino.h>
#include "communication.h"
#include "communication_legacy.h"
#include "pages.h"
#include "rt_data.h"

#define DEFAULT_CODE_VERSION "TSlib_11-2025"

class TS_lib {
  public:
    static char* PROTOCOL_VERSION = "002";

    TS_lib(const Stream* s1);
    TS_lib(const Stream* s1, const Rt_values* rt_values, const Page* pages, const uint16_t num_pages);
    void update(void);

    void setCodeVersion(const char* code_version);
    char* getCodeVersion(void);

    void setPages(const Page* pages, const uint16_t num_pages);
    void setRtData(const Rt_values* rt_values);

    Page* getPages(void);
    Page* getPage(const uint16_t pageNum);
    uint16_t getPageNum(void);

    Rt_values* getRtData(void);
  private:
    char* _code_version = DEFAULT_CODE_VERSION;
    const Stream* _serial1;
    const Communication _comm;
    const Communication_legacy _comm_legacy;
    const Pages _pages;
    const Rt_data _rt_data;
};

#endif