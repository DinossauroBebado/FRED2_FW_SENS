#ifndef can_h
#define can_h

#include "CAN.h"

#define CTX_GPIO_NUM   17  // Connects to CTX
#define CRX_GPIO_NUM   16  // Connects to CRX

enum reading_state {
  READING,
  DONE,
  NOT_RECEIVED 
}; 


struct CAN_MSG {
  long id;
  uint8_t rxData[8];
  int size;
  int read_flag;
};

typedef struct CAN_MSG can_msg ;

void canInit(int crx, int ctx, long baud);
void canSend(int id, const uint8_t *buffer, size_t size, int dlc = -1, bool rtr = false);
can_msg canReceive();

#endif