

#ifndef COMMUNICATION_LEGACY_H
#define COMMUNICATION_LEGACY_H

#include <Arduino.h>

#define DEFAULT_CODE_VERSION "TSlib_11-2025"
#define DEFAULT_PROTOCOL_VERSION "002"

class Communication_legacy {
	public:
    Communication_legacy() = default;
    Communication_legacy(const Stream* serial);
    Communication_legacy(const Stream* serial, const char* code_version);
    static bool isLegacy(uint8_t cmd);
		void serialReceiveLegacy();
  private:
    const char* _code_version;
    const char* _protocol_version;
    const Stream* _serial;
};

#endif