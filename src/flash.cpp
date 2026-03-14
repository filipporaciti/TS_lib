#include "flash.h"
#include <EEPROM.h>

void Flash::init(size_t size) {
  #ifdef ESP32
    EEPROM.begin(size);
  #endif
}

void Flash::write(size_t address, uint8_t value) {
  yield();
  EEPROM.write(address, value);
}

uint8_t Flash::read(size_t address) {
  yield();
  return EEPROM.read(address);
}

void Flash::commit() {
  #ifdef ESP32
    yield();
    EEPROM.commit();
  #endif
}