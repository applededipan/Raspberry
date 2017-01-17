
#include "task3_mavpddl.h"

mavlink_system_t mavlink_system = {RASP_SYSTEM_ID, RASP_COMPON_ID};
	
 
void usart_Send_Buffer(const char *buf, uint16_t len)
{
	int i;

	if (len > 0) {
		for ( i = 0; i < len; ++i) {
			serialPutchar(fd1, (unsigned char)buf[i]);
		}
		   
	}	
}


void task3_MavPddl(void)
{
	static uint8_t linkStatus = 0;	
	
	if (system("ping -c 2 192.168.42.123")) {			// camera ip addr
		
		if (system("ping -c 2 192.168.42.2")) {			// slave ip addr	
			
			if (system("ping -c 2 192.168.42.3")) {		// master ip addr
				
				linkStatus = 128;
				
			} else {
				linkStatus = 144;
			}
			
		} else {
			linkStatus = 176;
		}
		
	} else {
		linkStatus = 240;
	}
	
	// send to SmartConsole	
	mavlink_msg_debug_send(MAVLINK_COMM_0, 0, linkStatus, 0);

	// send linkStatus by udp to QGC
	mavlink_message_t debugMsg;
	uint8_t debugMsgBuffer[BUFFER_LENGTH];
	mavlink_msg_debug_pack(RASP_SYSTEM_ID, RASP_COMPON_ID, &debugMsg, 0, linkStatus, 0);     
	uint16_t lenMsg = mavlink_msg_to_send_buffer(debugMsgBuffer, &debugMsg);
	sendto(udpsock, debugMsgBuffer, lenMsg, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));
		
}


