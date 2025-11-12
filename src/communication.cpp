#include "communication.h"
#include "globals.h"
#include <CRC32.h>

// *********************************************
SerialStatus serialStatusFlag = SERIAL_READY;
uint16_t serialPayloadLen = 0;
unsigned long serialReceiveStartTime = 0;
uint16_t serialBufferIndex = 0;
uint8_t serialReceiveBuffer[SERIAL_BUFFER_SIZE] = {};

void processSerialPayload(Stream* s);
void sendMessage(Stream* s, uint8_t flag, const uint8_t *payload, uint16_t payloadLen);
void sendCodeVersion(Stream* s);
void sendSerialProtocolVersion(Stream* s);
void sendTestComm(Stream* s);
void sendPageValue(Stream* s);
void sendCRCPage(Stream* s);
void sendRealTimeData(Stream* s);
void sendSavePage(Stream* s);
void sendChangePageValue(Stream* s);
// --------------------------
void sendCodeMessage(Stream* s, uint8_t code);
// *********************************************


void serialReceive(Stream* s) {

	// if ((serialStatusFlag != SERIAL_READY || (serialStatusFlag == SERIAL_READY &&  s->available() == 1)) && (millis() - serialReceiveStartTime) > SERIAL_TIMEOUT) {
	// 	sendCodeMessage(s, SERIAL_MSG_TIMEOUT);
	// 	serialReceiveStartTime = millis();
	// 	while (s->available() > 0) {
	// 		s->read();
	// 	}
	// 	serialStatusFlag = SERIAL_READY;
	// }

	if (serialStatusFlag == SERIAL_READY && s->available() >= 2) {
		serialReceiveStartTime = millis();
		serialPayloadLen = (s->read() << 8 | s->read());
		serialBufferIndex = 0;
		serialStatusFlag = SERIAL_RECEIVE_PAYLOAD_INPROGRESS;
		
		if (serialPayloadLen > SERIAL_BUFFER_SIZE) { // se la lunghezza supero quella del buffer errore
			sendCodeMessage(s, SERIAL_MSG_RANGE_ERR);
			while (s->available() > 0) {
				s->read();
			}
			serialStatusFlag = SERIAL_READY;
		}
	}

	while (serialStatusFlag == SERIAL_RECEIVE_PAYLOAD_INPROGRESS && s->available() > 0 && serialBufferIndex < serialPayloadLen) {
		serialReceiveBuffer[serialBufferIndex] = s->read();
		serialBufferIndex++;

		if (serialBufferIndex == serialPayloadLen) {
			serialStatusFlag = SERIAL_RECEIVE_CRC_INPROGRESS;
		}
	}

	if (serialStatusFlag == SERIAL_RECEIVE_CRC_INPROGRESS && s->available() >= 4) {
		uint32_t crc = CRC32::calculate(serialReceiveBuffer, serialBufferIndex);

		if (s->read() != ((crc >> 24) & 0xFF) || 
		s->read() != ((crc >> 16) & 0xFF) || 
		s->read() != ((crc >> 8) & 0xFF) || 
		s->read() != (crc & 0xFF)) {
			sendCodeMessage(s, SERIAL_MSG_WRONG_CRC);
		} else {
			processSerialPayload(s);
		}

		serialStatusFlag = SERIAL_READY;
	}
}


void processSerialPayload(Stream* s) {

	switch (serialReceiveBuffer[0]) {
		case 'Q': // code version
			sendCodeVersion(s);
			break;
		case 'S': // string code version
			sendCodeVersion(s);
			break;
		case 'F': // protocol version
			sendSerialProtocolVersion(s);
			break;
		case 'C': // test communication
			sendTestComm(s);
			break;
		case 'p': // send page value: 1 page num | 2 offset | 2 len
			sendPageValue(s);
			break;
		case 'd': // senc crc page: 1 page num
			sendCRCPage(s);
			break;
		case 'A': // receive realtime data
			sendRealTimeData(s);
			break;
		case 'b': // burn page: 1 page num
			sendSavePage(s);
			break;
		case 'M': // change page value: 1 page num | 2 offset | 2 len | n values
			sendChangePageValue(s);
			break;
		default:
			sendCodeMessage(s, SERIAL_MSG_UKNW_COMMAND);
			break;
	}
}

void sendMessage(Stream* s, uint8_t flag, const uint8_t *payload, uint16_t payloadLen) {
}

void sendCodeMessage(Stream* s, uint8_t code) {
}

void sendCodeVersion(Stream* s) {
}
void sendSerialProtocolVersion(Stream* s) {
}
void sendTestComm(Stream* s) {
}



void sendPageValue(Stream* s) {
}

void sendCRCPage(Stream* s) {
}

void sendRealTimeData(Stream* s) {
}

void sendSavePage(Stream* s) {
}

void sendChangePageValue(Stream* s) {
}
