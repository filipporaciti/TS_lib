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

    TS_lib() = default;
    TS_lib(Stream* s1);
    TS_lib(Stream* s1, Rt_values* rt_values, Page* pages, uint16_t num_pages);
    void update(void);

    void setCodeVersion(char* code_version);
    char* getCodeVersion(void);

    void setPages(Page* pages, uint16_t num_pages);
    void setRtData(Rt_values* rt_values);

    Page* getPages(void);
    Page* getPage(uint16_t pageNum);
    uint16_t getPageNum(void);

    Rt_values* getRtData(void);
  private:
    char* _code_version = DEFAULT_CODE_VERSION;
    Stream* _serial1;
    Communication _comm;
    Communication_legacy _comm_legacy;
    Pages _pages;
    Rt_data _rt_data;
};

#endif