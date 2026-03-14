#ifndef FLASH_H
#define FLASH_H

#include <Arduino.h>

class Flash {
  public:
    Flash() = default;
    void init(size_t size);
    void write(size_t address, uint8_t value);
    uint8_t read(size_t address);
    void commit();
};

#endif