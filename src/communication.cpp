#include "communication.h"
#include "pages.h"
#include <CRC32.h>
#include <Arduino.h>


Communication::Communication(const Stream* serial) {
	Communication(serial, DEFAULT_CODE_VERSION);
}
Communication::Communication(const Stream* serial, const char* code_version) {
	_code_version = code_version;
	_protocol_version = DEFAULT_PROTOCOL_VERSION;
	_serial = serial;
}

void Communication::serialReceive() {

	// if ((serialStatusFlag != SERIAL_READY || (serialStatusFlag == SERIAL_READY &&  _serial->available() == 1)) && (millis() - serialReceiveStartTime) > SERIAL_TIMEOUT) {
	// 	sendCodeMessage(_serial, SERIAL_MSG_TIMEOUT);
	// 	serialReceiveStartTime = millis();
	// 	while (s->available() > 0) {
	// 		_serial->read();
	// 	}
	// 	serialStatusFlag = SERIAL_READY;
	// }

	if (serialStatusFlag == SERIAL_READY && _serial->available() >= 2) {
		serialReceiveStartTime = millis();
		serialPayloadLen = (_serial->read() << 8 | _serial->read());
		serialBufferIndex = 0;
		serialStatusFlag = SERIAL_RECEIVE_PAYLOAD_INPROGRESS;
		
		if (serialPayloadLen > SERIAL_BUFFER_SIZE) { // se la lunghezza supero quella del buffer errore
			sendCodeMessage(_serial, SERIAL_MSG_RANGE_ERR);
			while (_serial->available() > 0) {
				_serial->read();
			}
			serialStatusFlag = SERIAL_READY;
		}
	}

	while (serialStatusFlag == SERIAL_RECEIVE_PAYLOAD_INPROGRESS && _serial->available() > 0 && serialBufferIndex < serialPayloadLen) {
		serialReceiveBuffer[serialBufferIndex] = _serial->read();
		serialBufferIndex++;

		if (serialBufferIndex == serialPayloadLen) {
			serialStatusFlag = SERIAL_RECEIVE_CRC_INPROGRESS;
		}
	}

	if (serialStatusFlag == SERIAL_RECEIVE_CRC_INPROGRESS && _serial->available() >= 4) {
		uint32_t crc = CRC32::calculate(serialReceiveBuffer, serialBufferIndex);

		if (_serial->read() != ((crc >> 24) & 0xFF) || 
		_serial->read() != ((crc >> 16) & 0xFF) || 
		_serial->read() != ((crc >> 8) & 0xFF) || 
		_serial->read() != (crc & 0xFF)) {
			sendCodeMessage(_serial, SERIAL_MSG_WRONG_CRC);
		} else {
			processSerialPayload();
		}

		serialStatusFlag = SERIAL_READY;
	}
}


void Communication::processSerialPayload() {

	switch (serialReceiveBuffer[0]) {
		case 'Q': // code version
			sendCodeVersion();
			break;
		case 'S': // string code version
			sendCodeVersion();
			break;
		case 'F': // protocol version
			sendSerialProtocolVersion();
			break;
		case 'C': // test communication
			sendTestComm(_serial);
			break;
		case 'p': // send page value: 1 page num | 2 offset | 2 len
			sendPageValue();
			break;
		case 'd': // senc crc page: 1 page num
			sendCRCPage();
			break;
		case 'A': // receive realtime data
			sendRealTimeData(_serial);
			break;
		case 'b': // burn page: 1 page num
			sendSavePage(_serial);
			break;
		case 'M': // change page value: 1 page num | 2 offset | 2 len | n values
			sendChangePageValue(_serial);
			break;
		default:
			sendCodeMessage(_serial, SERIAL_MSG_UKNW_COMMAND);
			break;
	}
}

void Communication::sendMessage(const Stream* s, const uint8_t flag, const uint8_t *payload, const uint16_t payloadLen) {
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

void Communication::sendCodeMessage(const Stream* s, const uint8_t code) {
	uint8_t data[0] = {};
	sendMessage(s, code, data, sizeof(data));
}
void Communication::sendCodeVersion() {
	sendMessage(_serial, SERIAL_MSG_SUCCESS, _code_version, sizeof(_code_version)-1); // -1 per il terminatore finale
}
void Communication::sendSerialProtocolVersion() {
	sendMessage(_serial, SERIAL_MSG_SUCCESS, _protocol_version, sizeof(_protocol_version)-1);
}
void Communication::sendTestComm(const Stream* s) {
	uint8_t data[] = {0xFF};
	sendMessage(s, SERIAL_MSG_SUCCESS, data, sizeof(data));
}



void Communication::sendPageValue() {
	uint16_t pageNum = (uint16_t)serialReceiveBuffer[1];
	uint16_t offset = (serialReceiveBuffer[3] << 8 | serialReceiveBuffer[2]); // little endian
	uint16_t len = (serialReceiveBuffer[5] << 8 | serialReceiveBuffer[4]);		// little endian

	void* first_byte = getPageValue(pageNum, offset);
	if (first_byte == nullptr) {
		sendCodeMessage(_serial, SERIAL_MSG_RANGE_ERR);
		return;
	}

	uint8_t data[len] = {};

	memcpy(data, first_byte, len);
	sendMessage(_serial, SERIAL_MSG_SUCCESS, data, sizeof(data));
}

void Communication::sendCRCPage() {
	uint16_t pageNum = (uint16_t)serialReceiveBuffer[1];

	void* first_byte = getPageValue(pageNum, 0); // check pageNum
	if (first_byte == nullptr) {
		sendCodeMessage(_serial, SERIAL_MSG_RANGE_ERR);
		return;
	}

	size_t pageLen = getPageLen(pageNum);
	uint32_t crc = getPageCRC(first_byte, pageLen);

  uint8_t data[4];

  data[0] = (crc >> 24) & 0xFF;
	data[1] = (crc >> 16) & 0xFF;
	data[2] = (crc >> 8) & 0xFF;
	data[3] = crc & 0xFF;   

	sendMessage(_serial, SERIAL_MSG_SUCCESS, data, sizeof(data));
}

void Communication::sendRealTimeData(const Stream* s) {
}

void Communication::sendSavePage(const Stream* s) {
}

void Communication::sendChangePageValue(const Stream* s) {
}
