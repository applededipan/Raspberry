
#include "task3_mavpddl.h"

mavlink_system_t mavlink_system = {RASP_SYSTEM_ID, RASP_COMPON_ID};
	
const char path[3][38] = {
"/home/pi/Firmware/shell/pingTarget.sh",
"/home/pi/Firmware/shell/pingSlave.sh",
"/home/pi/Firmware/shell/pingMaster.sh",
};

unsigned int task3_Ping(const char *path)
{
	static  char commandId;
	static  FILE *fp3;	

	fp3 = popen(path, "r");
	fgets(&commandId, sizeof(&commandId), fp3);
	pclose(fp3);

	return atoi(&commandId);
}

 
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

	if (task3_Ping(path[0])) {

		if (task3_Ping(path[1])) {
			
			if (task3_Ping(path[2])) {
				linkStatus = 128;	// 0X80   linked with rasp
				
			} else {
				linkStatus = 144;	// 0X90   linked with master
			}

		} else {
			linkStatus = 176;		// 0XB0   linked with salve 
		}
		
	} else {
		linkStatus = 240;			// 0XF0   linked with camera 
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


