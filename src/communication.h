
/*
*/
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>

#define DEFAULT_CODE_VERSION "TSlib_11-2025"
#define DEFAULT_PROTOCOL_VERSION "002"


#define SERIAL_BUFFER_SIZE        256
#define SERIAL_TIMEOUT            2000

#define SERIAL_MSG_SUCCESS        0x00
#define SERIAL_MSG_BURN_SUCCESS   0x04
#define SERIAL_MSG_TIMEOUT        0x80
#define SERIAL_MSG_WRONG_CRC      0x82
#define SERIAL_MSG_UKNW_COMMAND   0x83
#define SERIAL_MSG_RANGE_ERR      0x84


class Communication {
  public:
    Communication() = default;
    Communication(const Stream* serial);
    Communication(const Stream* serial, const char* code_version);
    void serialReceive();
  private:
    enum SerialStatus { SERIAL_READY, SERIAL_RECEIVE_PAYLOAD_INPROGRESS,SERIAL_RECEIVE_CRC_INPROGRESS, };

    const char* _code_version;
    const char* _protocol_version;
    const Stream* _serial;

    SerialStatus serialStatusFlag = SERIAL_READY;
    uint16_t serialPayloadLen = 0;
    unsigned long serialReceiveStartTime = 0;
    uint16_t serialBufferIndex = 0;
    uint8_t serialReceiveBuffer[SERIAL_BUFFER_SIZE] = {};

    void processSerialPayload();
    void sendCodeVersion();
    void sendSerialProtocolVersion();
    void sendPageValue();
    void sendCRCPage();
    void sendRealTimeData();
    static void sendMessage(const Stream* s, const uint8_t flag, const uint8_t *payload, const uint16_t payloadLen);
    static void sendTestComm(const Stream* s);
    static void sendSavePage(const Stream* s);
    static void sendChangePageValue(const Stream* s);
    // --------------------------
    static void sendCodeMessage(const Stream* s, const uint8_t code);

};

#endif

