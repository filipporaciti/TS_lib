#include <Arduino.h>


#define SERIAL_BUFFER_SIZE        256
#define SERIAL_TIMEOUT            2000

#define SERIAL_MSG_SUCCESS        0x00
#define SERIAL_MSG_BURN_SUCCESS   0x04
#define SERIAL_MSG_TIMEOUT        0x80
#define SERIAL_MSG_WRONG_CRC      0x82
#define SERIAL_MSG_UKNW_COMMAND   0x83
#define SERIAL_MSG_RANGE_ERR      0x84


enum SerialStatus {
  /**  */
  SERIAL_READY, 
  /**  */
  SERIAL_RECEIVE_PAYLOAD_INPROGRESS,
  /**  */
  SERIAL_RECEIVE_CRC_INPROGRESS,
};

extern SerialStatus serialStatusFlag;
extern uint16_t serialPayloadLen;
extern unsigned long serialReceiveStartTime;
extern uint8_t serialReceiveBuffer[SERIAL_BUFFER_SIZE];
extern uint16_t serialBufferIndex;

void serialReceive(const Stream* s);

