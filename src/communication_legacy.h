

#ifndef COMMUNICATION_LEGACY_H
#define COMMUNICATION_LEGACY_H

#include <Arduino.h>

#define DEFAULT_PROTOCOL_VERSION "002"

class Communication_legacy {
	public:
    Communication_legacy() = default;
    Communication_legacy(Stream* serial);
    Communication_legacy(Stream* serial, char* code_version);
    static bool isLegacy(uint8_t cmd);
		void serialReceiveLegacy();
    void setCodeVersion(char* code_version);
  private:
    char* _code_version;
    const char* _protocol_version;
    Stream* _serial;
};

#endif