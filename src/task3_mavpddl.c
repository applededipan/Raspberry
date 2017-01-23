
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


bool systemPing(char *ip)
{
    bool state;
	FILE *fp = NULL;
	char cmd[1024];

    if ((fp = fopen("ping.txt", "wt+")) == NULL) {
        printf("apple: cerat ping.txt file failed \n");
        return false;
    }

    if (sprintf(cmd, "ping -c 1 %s > ping.txt", ip) == -1) {
        printf("apple: sprintf failed \n");
        return false;
    }
    
    system(cmd);

    fp = fopen("ping.txt", "r");
    
    if (fp == NULL) {
        printf("apple: fopen ping.txt file failed \n");
        return false;

    } else {
        uint8_t i;
        for(i = 0; i < 2; i++) {
            fgets(cmd, sizeof(cmd), fp);
        }
                
        char *p;
        if ((p = strstr(cmd, "ttl")) == NULL) {
            printf("apple: ping %s failed \n", ip);
            state = false;
        } else {

            state = true;
        }
        fclose(fp);
    }
    
    system("rm -f ping.txt");
    
    return state;
    
}


void task3_MavPddl(void)
{
	static uint8_t linkStatus = 128;	
	
	if (systemPing("192.168.42.123") == false) {				// camera ip addr
		
		if (systemPing("192.168.42.2") == false) {			// slave ip addr	
			
			if (systemPing("192.168.42.3") == false) {		// master ip addr
				
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


