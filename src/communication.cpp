#include "communication.h"
#include "TS_lib.h"
#include <CRC32.h>

// *********************************************
SerialStatus serialStatusFlag = SERIAL_READY;
uint16_t serialPayloadLen = 0;
unsigned long serialReceiveStartTime = 0;
uint16_t serialBufferIndex = 0;
uint8_t serialReceiveBuffer[SERIAL_BUFFER_SIZE] = {};

void processSerialPayload(const Stream* s);
void sendMessage(const Stream* s, const uint8_t flag, const uint8_t *payload, const uint16_t payloadLen);
void sendCodeVersion(const Stream* s);
void sendSerialProtocolVersion(const Stream* s);
void sendTestComm(const Stream* s);
void sendPageValue(const Stream* s);
void sendCRCPage(const Stream* s);
void sendRealTimeData(const Stream* s);
void sendSavePage(const Stream* s);
void sendChangePageValue(const Stream* s);
// --------------------------
void sendCodeMessage(const Stream* s, const uint8_t code);
// *********************************************


void serialReceive(const Stream* s) {

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


void processSerialPayload(const Stream* s) {

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

void sendMessage(const Stream* s, const uint8_t flag, const uint8_t *payload, const uint16_t payloadLen) {
	uint8_t header[2];
  header[0] = (payloadLen + 1) >> 8;   // +1 per il flag
  header[1] = (payloadLen + 1) & 0xFF;

  // Calcola CRC anche includendo il flag
  uint32_t crc;
  
  CRC32 crcCalc;
  crcCalc.update(&flag, 1);
  crcCalc.update(payload, payloadLen);
  crc = crcCalc.finalize();

  // --- Invio ---
  s->write(header, 2);        // dimensione totale (2 byte)
  s->write(&flag, 1);
  s->write(payload, payloadLen); // dati veri e propri
  s->write((crc >> 24) & 0xFF);
  s->write((crc >> 16) & 0xFF);
  s->write((crc >> 8) & 0xFF);
  s->write(crc & 0xFF);
  s->flush();
}

void sendCodeMessage(const Stream* s, const uint8_t code) {
	uint8_t data[0] = {};
	sendMessage(s, code, data, sizeof(data));
}
void sendCodeVersion(const Stream* s) {
	sendMessage(s, SERIAL_MSG_SUCCESS, TS_lib::CODE_VERSION, sizeof(TS_lib::CODE_VERSION)-1); // -1 per il terminatore finale
}
void sendSerialProtocolVersion(const Stream* s) {
	sendMessage(s, SERIAL_MSG_SUCCESS, TS_lib::PROTOCOL_VERSION, sizeof(TS_lib::PROTOCOL_VERSION)-1);
}
void sendTestComm(const Stream* s) {
	uint8_t data[] = {0xFF};
	sendMessage(s, SERIAL_MSG_SUCCESS, data, sizeof(data));
}



void sendPageValue(const Stream* s) {
}

void sendCRCPage(const Stream* s) {
}

void sendRealTimeData(const Stream* s) {
}

void sendSavePage(const Stream* s) {
}

void sendChangePageValue(const Stream* s) {
}
