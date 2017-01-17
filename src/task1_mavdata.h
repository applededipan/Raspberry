#ifndef TASK1_MAVDATA_H
#define TASK1_MAVDATA_H


#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

#if (defined __QNX__) | (defined __QNXNTO__)
/* QNX specific headers */
#include <unix.h>
#else
/* Linux / MacOS POSIX timer headers */
#include <sys/time.h>
#include <time.h>
#include <arpa/inet.h>
#endif

#include <inttypes.h> // For fixed-width uint8_t type
#include "global.h"


enum FTP_OPCODE
{
	kCmdNone,				//! ignored, always acked
	kCmdTerminateSession,	//! Terminates open Read session
	kCmdResetSessions,		//! Terminates all open Read sessions
	kCmdListDirectory,		//! List files in <path> from <offset>
	kCmdOpenFileRO,			//! Opens file at <path> for reading, returns <session>
	kCmdReadFile,			//! Reads <size> bytes from <offset> in <session>
	kCmdCreateFile,			//! Creates file at <path> for writing, returns <session>
	kCmdWriteFile,			//! Writes <size> bytes to <offset> in <session>
	kCmdRemoveFile,			//! Remove file at <path>
	kCmdCreateDirectory,	//! Creates directory at <path>
	kCmdRemoveDirectory,	//! Removes Directory at <path>, must be empty
	kCmdOpenFileWO,			//! Opens file at <path> for writing, returns <session>
	kCmdTruncateFile,		//! Truncate file at <path> to <offset> length
	kCmdRename,				//! Rename <path1> to <path2>
	kCmdCalcFileCRC32,		//! Calculate CRC32 for file at <path>
	kCmdBurstReadFile,      //! Burst download session file
	kCmdSearchVersion,      //! search for firmware version by wireless connection
	kCmdReboot = 100,       //! reBoot
	kRspAck = 128,          //! Ack response
	kRspNak,                //! Nak response
	kCmdTestNoAck,          //! ignored, ack not sent back, should timeout waiting for ack
};


typedef struct 
{
	uint16_t seqNumber;      //! sequence number for message
	uint8_t  session;        //! Session id for read and write commands
	uint8_t  opcode;         //! Command opcode
	uint8_t  size;           //! Size of data
	uint8_t  reqOpcode;      //! Request opcode returned in kRspAck, kRspNak message
	uint8_t  burstComplete;  //! Only used if req_opcode=kCmdBurstReadFile - 1: set of burst packets complete, 0: More burst packets coming.
	uint8_t  padding;        //! 32 bit aligment padding
	uint32_t offset;         //! Offsets for List and Read commands
}RequestHeader;	


typedef struct 
{
	RequestHeader hdr;
	uint8_t data[sizeof(((mavlink_file_transfer_protocol_t*)0)->payload) - sizeof(RequestHeader)];			
}Payload;	


typedef struct 
{
	uint8_t network;  //! Network ID   (0 for broadcast)
	uint8_t sysid;    //! System ID    (0 for broadcast)
	uint8_t cmpid;    //! Component ID (0 for broadcast)
	Payload payload;
}Ftp;


Ftp ftp;
bool firmwareUpdate;



void ftpProcess(void);
void task1_MavData(void);
bool creatUpdateFile(uint8_t* data, uint32_t offset, uint8_t size);
int udpinit(char *target_addr,struct sockaddr_in *a, struct sockaddr_in *b);
bool handle_message_file_transfer_protocol(const mavlink_message_t *msg); 


#endif 
