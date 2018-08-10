
#ifndef _MQTT_HELPER_H
#define _MQTT_HELPER_H

#include "board.h"


bool transport_open(void);
bool transport_close(void);


bool ethe_send_buffer(char *pMsg, uint16_t msg_len);
void ethe_read_buffer(void);






#endif
