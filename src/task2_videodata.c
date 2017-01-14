
#include "task2_videodata.h"

void task2_status_network(char *hw_name,unsigned int *status)
{
	FILE *fp1_task2;
	
	memset(buf_task2, 0, sizeof(buf_task2));   
	snprintf(carrier_path, sizeof(carrier_path), "/sys/class/net/%s/carrier", hw_name);  

	if ((fp1_task2 = fopen(carrier_path, "r")) != NULL) {  
		while (fgets(buf_task2, sizeof(buf_task2), fp1_task2) != NULL) {  
			if (buf_task2[0]=='0') {
				*status = IFSTATUS_DOWN; 
			} else {
				*status = IFSTATUS_UP;  
			}
		} 

	} else {		
		perror("Open carrier "); 
	}

	fclose(fp1_task2);  
}


void task2_videdata()    
{
	FILE *fp0_task2;
	
	if ((fp0_task2 = fopen("/proc/net/dev", "r"))!=NULL) {   
		memset(buf0_task2, 0, sizeof(buf0_task2)); 
		
		while (fgets(buf0_task2, sizeof(buf0_task2), fp0_task2) != NULL) {  
			if (strstr(buf0_task2, "usb0") != NULL) {         
				task2_status_network("usb0", &flag_usb0);
				break;
				
			} else {
				flag_usb0 = 0xcc;
			}

			if (strstr(buf0_task2, "eth0") != NULL) {         
				task2_status_network("eth0",&flag_eth0);
				
				if (flag_eth0 == IFSTATUS_UP) {
					break;
				}    
			 
			} else {
				flag_eth0 = 0xcc; 
			}     
		}  
	}  

	fclose(fp0_task2); 

	//=============
	  
	if (flag_eth0 == IFSTATUS_UP && flag_usb0 == IFSTATUS_UP) {
		flag_network = ETH0_UP;
		
	} else if (flag_usb0 == IFSTATUS_UP) {
		flag_network = USB0_UP;
		
	} else if (flag_eth0 == IFSTATUS_UP) {
		
		flag_network = ETH0_UP;
		
	} else if (flag_eth0 != IFSTATUS_UP && flag_usb0 != IFSTATUS_UP) { 
		flag_network = NOT_LINK;
		printf("no link \n");
	}

	//====

	if (flag_network == USB0_UP) {
		if (FlagUsb0Uped == 0) {
			printf("usb0 is up \n");
			system("br1down");
			system("br0up");
			FlagUsb0Uped = 1;
		}
		
	} else {
		FlagUsb0Uped = 0;
	}

	//============

	if (flag_network == ETH0_UP) {
		if (FlagEth0Uped == 0) {
			printf("eth0 is up \n");
			system("br0down");
			system("br1up");
			FlagEth0Uped=1;
		}

	} else {
		FlagEth0Uped = 0;
	} 
}
  



