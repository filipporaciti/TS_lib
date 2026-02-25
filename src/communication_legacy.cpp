#include "communication_legacy.h"
#include "TS_lib.h"


Communication_legacy::Communication_legacy(Stream* serial) {
	_code_version = DEFAULT_CODE_VERSION;
	_protocol_version = DEFAULT_PROTOCOL_VERSION;
	_serial = serial;
}

Communication_legacy::Communication_legacy(Stream* serial, char* code_version) {
	_code_version = code_version;
	_protocol_version = DEFAULT_PROTOCOL_VERSION;
	_serial = serial;
}

void Communication_legacy::serialReceiveLegacy() {

	if (_serial->available()) {
		char serialCmd = (char)_serial->read();
		switch (serialCmd) {
			case 'Q': {
				_serial->print(_code_version);
				_serial->flush();
				break;
			}
			case 'S': {
				_serial->print(_code_version);
				_serial->flush();
				break;
			}
			case 'F': {
				_serial->print(_protocol_version);
				_serial->flush();
				break;
			}
		}
	}
}

bool Communication_legacy::isLegacy(uint8_t cmd) {
	uint8_t cmds[] = {'Q', 'S', 'F'};
	for (uint8_t i=0; i<sizeof(cmds); i++) {
		if (cmds[i] == cmd) {
			return true;
		}
	}
	return false;
}


void Communication_legacy::setCodeVersion(char* code_version) {
	_code_version = code_version;
}