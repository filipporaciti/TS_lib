#include "communication.h"
#include "pages.h"
#include "rt_data.h"
#include "rt_data.h"
#include <CRC32.h>
#include <Arduino.h>


Communication::Communication(Stream* serial, char* code_version, Rt_data* rt_data, Pages* pages) {
	_code_version = code_version;
	_protocol_version = DEFAULT_PROTOCOL_VERSION;
	_serial = serial;
	_pages = pages;
	_rt_data = rt_data;
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
		
		if (serialPayloadLen > SERIAL_BUFFER_SIZE) {
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

		if ((uint8_t)_serial->read() != ((crc >> 24) & 0xFF) || 
		(uint8_t)_serial->read() != ((crc >> 16) & 0xFF) || 
		(uint8_t)_serial->read() != ((crc >> 8) & 0xFF) || 
		(uint8_t)_serial->read() != (crc & 0xFF)) {
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
			sendRealTimeData();
			break;
		case 'b': // burn page: 1 page num
			sendSavePage();
			break;
		case 'M': // change page value: 1 page num | 2 offset | 2 len | n values
			sendChangePageValue();
			break;
		case 'I': // send can ID
			sendCanID();
			break;
		case 'f': // send can info (serial version - table blocking factor - write blocking factor)
			sendCanInfo();
			break;
		default:
			sendCodeMessage(_serial, SERIAL_MSG_UKNW_COMMAND);
			break;
	}
}

void Communication::sendMessage(Stream* s, uint8_t flag, uint8_t *payload, unsigned long payloadLen) {
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

void Communication::sendCodeMessage(Stream* s, uint8_t code) {
	uint8_t data[0] = {};
	sendMessage(s, code, data, sizeof(data));
}
void Communication::sendCodeVersion() {
	sendMessage(_serial, SERIAL_MSG_SUCCESS, (uint8_t*)_code_version, strlen(_code_version));
}
void Communication::sendSerialProtocolVersion() {
	sendMessage(_serial, SERIAL_MSG_SUCCESS, (uint8_t*)_protocol_version, strlen(_protocol_version));
}
void Communication::sendTestComm(Stream* s) {
	uint8_t data[] = {0xFF};
	sendMessage(s, SERIAL_MSG_SUCCESS, data, sizeof(data));
}
void Communication::sendCanID() {
	sendMessage(_serial, SERIAL_MSG_SUCCESS, &_canID, 1);
}

void Communication::sendCanInfo() {
	if (serialReceiveBuffer[1] == _canID) {
		uint8_t payload[5];
		payload[0] = SERIAL_VERSION;
		payload[1] = (TABLE_BLOCKING_FACTOR >> 8);
		payload[2] = (TABLE_BLOCKING_FACTOR) & 0xFF;
		payload[3] = (WRITE_BLOCKING_FACTOR >> 8);
		payload[4] = (WRITE_BLOCKING_FACTOR) & 0xFF;

		sendMessage(_serial, SERIAL_MSG_SUCCESS, payload, sizeof(payload));
	}
}


void Communication::sendPageValue() {
	uint16_t pageNum = (uint16_t)serialReceiveBuffer[1];
	uint16_t offset = (serialReceiveBuffer[3] << 8 | serialReceiveBuffer[2]); // little endian
	uint16_t len = (serialReceiveBuffer[5] << 8 | serialReceiveBuffer[4]);		// little endian

	void* first_byte = _pages->getPageValue(pageNum, offset);
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

	if (_pages->isIndexOutOfRange(pageNum)) {
		sendCodeMessage(_serial, SERIAL_MSG_RANGE_ERR);
		return;
	}

	uint32_t crc = _pages->getPageCRC(pageNum);

	if (crc == 0) {
		sendCodeMessage(_serial, SERIAL_MSG_WRONG_CRC);
		return;
	}

	uint8_t data[4];

	data[0] = (crc >> 24) & 0xFF;
	data[1] = (crc >> 16) & 0xFF;
	data[2] = (crc >> 8) & 0xFF;
	data[3] = crc & 0xFF;   

	sendMessage(_serial, SERIAL_MSG_SUCCESS, data, sizeof(data));
}

void Communication::sendRealTimeData() {
	uint8_t data[_rt_data->getRtDataLen()] = {};

	if (_rt_data->getRtData() != nullptr) {
		memcpy(data, _rt_data->getRtData(), _rt_data->getRtDataLen());
	}

	sendMessage(_serial, SERIAL_MSG_SUCCESS, data, sizeof(data));
}

void Communication::sendSavePage() {
	uint16_t pageNum = (uint16_t)serialReceiveBuffer[1];
	bool ris = _pages->storePage(pageNum);
	if (ris) {
		sendCodeMessage(_serial, SERIAL_MSG_BURN_SUCCESS);
	} else {
		sendCodeMessage(_serial, SERIAL_MSG_UKNW_COMMAND); // non esiste un code message specifico in caso fallisce il burn
	}
}

void Communication::sendChangePageValue() {
	uint16_t pageNum = (uint16_t)serialReceiveBuffer[1];
	uint16_t offset = (serialReceiveBuffer[3] << 8 | serialReceiveBuffer[2]); // little endian
	size_t len = (serialReceiveBuffer[5] << 8 | serialReceiveBuffer[4]);		// little endian

	void* first_byte = _pages->getPageValue(pageNum, offset);
	size_t pageLen = _pages->getPageLen(pageNum);

	if (first_byte == nullptr || (len + offset) > pageLen) {
		sendCodeMessage(_serial, SERIAL_MSG_RANGE_ERR);
		return;
	}

	uint8_t* page_data = (uint8_t*) first_byte;
	for (size_t i=0; i<len; i++) {
		page_data[len-1-i] = serialReceiveBuffer[6+i];
	}


	sendCodeMessage(_serial, SERIAL_MSG_SUCCESS);
}

void Communication::setCodeVersion(char* code_version) {
	_code_version = code_version;
}

bool Communication::isReady(void) {
	return serialStatusFlag == SERIAL_READY;
}