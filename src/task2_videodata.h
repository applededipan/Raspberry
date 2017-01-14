#ifndef TASK2_VIDEODATA_H
#define TASK2_VIDEODATA_H

#include <linux/sockios.h>  
#include <sys/socket.h>  
#include <sys/ioctl.h>  
#include <linux/if.h>  
#include <string.h>  
#include <stdio.h>
#include <stdbool.h>  
#include <stdlib.h>  
#include <unistd.h>  
  
#define ETHTOOL_GLINK        0x0000000a /* Get link status (ethtool_value) */  

#define ETH0_UP 1
#define USB0_UP 2
#define NOT_LINK 3

#define IFSTATUS_UP 0
#define IFSTATUS_DOWN 1
  
typedef signed int u32;  

  
  

// interface_status_t status;  
char buf_task2[512] = {'\0'};  
char buf0_task2[512] = {'\0'};  
char carrier_path[512] = {'\0'};  

// ==========
int FlagEth0Uped = 0;
int FlagUsb0Uped = 0;

/* for passing single values */  
struct ethtool_value  
{  
	u32 cmd;  
	u32 data;  
};  

unsigned int flag_eth0 = 0xcc;
unsigned int flag_usb0 = 0xcc;
unsigned int flag_network;

#endif
