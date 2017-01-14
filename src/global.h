#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdbool.h>

#define QGCONTROL_ID  255
#define RASP_SYSTEM_ID 250
#define RASP_COMPON_ID 100
#define RASP_VERSION   "V2.0.0"
#define BUFFER_LENGTH 1024

#define MAVLINK_USE_CONVENIENCE_FUNCTIONS

#include "../lib/mavlink/2.0/mavlink_types.h"
extern mavlink_system_t mavlink_system;
extern void usart_Send_Buffer(const char *buf, uint16_t len);

#define MAVLINK_SEND_UART_BYTES(chan,buf,len) usart_Send_Buffer(buf,len)

#include "../lib/mavlink/2.0/common/mavlink.h"
#include "../lib/mavlink/2.0/mavlink_helpers.h"

extern int fd1;
extern int udpsock;
extern struct sockaddr_in gcAddr;
extern struct sockaddr_in locAddr;

#endif
