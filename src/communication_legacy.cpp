#include "communication_legacy.h"
#include "globals.h"

void serialReceiveLegacy(Stream* s) {

	if (s->available() > 0) {
		uint8_t serialCmd = s->read();
		switch (serialCmd) {
			case 'Q': {
				s->print(CODE_VERSION);
				s->flush();
				break;
			}
			case 'S': {
				s->print(CODE_VERSION);
				s->flush();
				break;
			}
			case 'F': {
				s->print(PROTOCOL_VERSION);
				s->flush();
				break;
			}
		}
	}
}

bool isLegacy(uint8_t cmd) {
	uint8_t cmds[] = {'Q', 'S', 'F'};
	for (uint8_t i=0; i<sizeof(cmds); i++) {
		if (cmds[i] == cmd) {
			return true;
		}
	}
	return false;
}