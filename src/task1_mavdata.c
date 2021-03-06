
#include "task1_mavdata.h"


int udpinit(char *target_addr,struct sockaddr_in *a, struct sockaddr_in *b)  //a---gcAddr   b---locAddr
{
	char toip[100];
	int sock;
	
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);
		exit(0);
	}

	// Change the target ip if parameter was given
	strcpy(toip, target_addr);

	memset(b, 0, sizeof(*b));
	b->sin_family = AF_INET;
	b->sin_addr.s_addr = htonl(INADDR_ANY);
	b->sin_port = htons(14551);

	/* Bind the socket to port 14551 - necessary to receive packets from qgroundcontrol */ 
	if (bind(sock, (struct sockaddr *)b, sizeof(struct sockaddr)) == -1) {
		printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
		exit(0);	  
		// perror("error bind failed");
		// close(sock);
		// exit(EXIT_FAILURE);
	} 

	/* Attempt to make it non blocking */
	if (fcntl(sock, F_SETFL, O_NONBLOCK | FASYNC)<0) {
	  
	 fprintf(stderr, "error setting nonblocking: %s\n", strerror(errno));
	 close(sock);
	 exit(EXIT_FAILURE);
	}

	memset(a, 0, sizeof(*a));
	a->sin_family = AF_INET;
	a->sin_addr.s_addr = inet_addr(toip);
	a->sin_port = htons(14550);
	return sock;  

}


void task1_MavData(void)
{
	int i;
	static uint16_t cnt;
	static socklen_t fromlen;
	static uint8_t sendBuffer[BUFFER_LENGTH];
	static uint8_t recvBuffer[BUFFER_LENGTH];	
	static mavlink_message_t msgFromSmartConsole;
	static mavlink_status_t  statusFromSmartConsole;
	static mavlink_message_t msgUpdate;
	static mavlink_status_t  statusUpdate;

	// data from SmartConsole to QGC
	while (serialDataAvail(fd1)) {
		uint8_t byte = serialGetchar(fd1);

		if (mavlink_parse_char(MAVLINK_COMM_0, byte, &msgFromSmartConsole, &statusFromSmartConsole)) {			
		   uint16_t len = mavlink_msg_to_send_buffer(sendBuffer, &msgFromSmartConsole);
		   sendto(udpsock, sendBuffer, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));
		}
	}

	// data from QGC to SmartConsole
	int recsize = recvfrom(udpsock, recvBuffer, BUFFER_LENGTH, 0, (struct sockaddr *)&gcAddr, &fromlen);
	   
	if (recsize > 0) {

		for (i = 0; i < recsize; ++i) {
			serialPutchar(fd1, (unsigned char)recvBuffer[i]);

			if (mavlink_parse_char(MAVLINK_COMM_1, (unsigned char)recvBuffer[i], &msgUpdate, &statusUpdate)) {

				if (msgUpdate.msgid == MAVLINK_MSG_ID_FILE_TRANSFER_PROTOCOL) {

					if (handle_message_file_transfer_protocol(&msgUpdate) == 0) {
						// message ok
						ftpProcess();
					}
					 
				}
		   }

		}

	}

	usleep(1000);
}







/*******************************************************************************
 * @MSGID: MAVLINK_MSG_ID_FILE_TRANSFER_PROTOCOL 110   
  
 * @payload  Variable length payload.
 * @return   1:failed   0:ok
*******************************************************************************/
uint8_t handle_message_file_transfer_protocol(const mavlink_message_t *msg) 
{    
	if (msg->sysid != QGCONTROL_ID) return 1;  //! message must from QGC 

	mavlink_file_transfer_protocol_t data;
	mavlink_msg_file_transfer_protocol_decode(msg, &data);  
	Payload* payload = (Payload*)&data.payload[0];  
				   
	ftp.sysid = mavlink_msg_file_transfer_protocol_get_target_system(msg);
	if(ftp.sysid != RASP_SYSTEM_ID) return 1; //! see if this message is for raspberry            
								
	ftp.payload.hdr.seqNumber = payload->hdr.seqNumber;
	ftp.payload.hdr.opcode    = payload->hdr.opcode;
	ftp.payload.hdr.offset    = payload->hdr.offset; 
	ftp.payload.hdr.size      = payload->hdr.size;
	memcpy(&ftp.payload.data, &payload->data, sizeof(ftp.payload.data));

	return 0;                                                    
}




/******************************************************************************
 * @brief creat a bin file added by apple 2017/01/13

 * @brief *data the addr of received bin data from mavlink
 * @brief size  normal is 239

 * @return 0:ok  others:failed
******************************************************************************/
uint8_t creatUpdateFile(uint8_t *data, uint32_t offset, uint8_t size)
{
	FILE *fp;
	char buffer[256]; // actually only use first 239 bytes
	uint16_t count;
	memcpy(buffer, (char*)data, size);
	memset(data, 0, size);

	if ((fp = fopen("/home/pi/Firmware/src/main_temp", "wb+")) == NULL) {
	 
		return 1; // failed
		fclose(fp);

	}

	if (fseek(fp, offset, SEEK_END)) {
	 
		return 2;
		fclose(fp);

	}

	if (fwrite(buffer, size, count, fp) != count) {

		return 3;
		fclose(fp);

	}

	fclose(fp);

	return 0;

}




/*****************************************************************************
 * @brief process the ftp message   added by apple 2017/01/13

 * @brief note that only when write file failed the rasp does not response to QGC, otherwise must response to QGC

*****************************************************************************/
void ftpProcess(void)
{
	Ftp myFtp;
	myFtp.network = 0;
	myFtp.sysid   = RASP_SYSTEM_ID;
	myFtp.cmpid   = RASP_COMPON_ID;
	uint8_t ack   = 1;

	myFtp.payload.hdr.size    = sizeof(uint32_t);
	myFtp.payload.hdr.session = 0;
	myFtp.payload.hdr.opcode  = 128;
	myFtp.payload.hdr.offset  = ftp.payload.hdr.offset;
	myFtp.payload.hdr.reqOpcode = ftp.payload.hdr.opcode;
	myFtp.payload.hdr.seqNumber = ftp.payload.hdr.seqNumber + 1;
	memset(&myFtp.payload.data, 0, sizeof(myFtp.payload.data));
	*((uint32_t*)myFtp.payload.data) = ftp.payload.hdr.size;

	mavlink_message_t msgFtp;
	uint8_t msgFtpBuffer[BUFFER_LENGTH];

	switch(ftp.payload.hdr.opcode)
	{
		case kCmdCreateFile:
			break;

		case kCmdWriteFile:
			if (firmwareUpdate != 1) {

				firmwareUpdate = 1; // start firmware update!

			} else {

				if (creatUpdateFile((uint8_t*)&ftp.payload.data, ftp.payload.hdr.offset, ftp.payload.hdr.size)) {
				   
					ack = 0; // write file failed!

				} else {

					ack = 1; // write file succeed!
				}
			}
			break;

			
		case kCmdResetSessions:
			break;

			
		case kCmdSearchVersion:
			strcpy((char*)myFtp.payload.data, RASP_VERSION);
			break;
		
		case kCmdRemoveFile:
			break;
		
		case kCmdReboot:
			mavlink_msg_file_transfer_protocol_pack(RASP_SYSTEM_ID, RASP_COMPON_ID, &msgFtp, myFtp.network, QGCONTROL_ID, 0, (uint8_t*)&myFtp.payload);
			uint16_t lenMsg = mavlink_msg_to_send_buffer(msgFtpBuffer, &msgFtp);
			sendto(udpsock, msgFtpBuffer, lenMsg, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));
			system("sudo mv -f /home/pi/Firmware/src/main_temp /home/pi/Firmware/src/main_update");
			system("sudo reboot");
			break;
				
			
		default:
			break;

	}

	// only when write file failed we will not response, otherwise we all have to response to the ftp message!
	if (ack) {
		mavlink_msg_file_transfer_protocol_pack(RASP_SYSTEM_ID, RASP_COMPON_ID, &msgFtp, myFtp.network, QGCONTROL_ID, 0, (uint8_t*)&myFtp.payload);
		uint16_t len = mavlink_msg_to_send_buffer(msgFtpBuffer, &msgFtp);
		sendto(udpsock, msgFtpBuffer, len, 0, (struct sockaddr*)&gcAddr, sizeof(struct sockaddr_in));
	}


}



















