#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include <signal.h>

//=======================================  network
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

//=======================================
#include <inttypes.h> // For fixed-width uint8_t type

//=======================================
#include "global.h"
#include "wiringSerial.h"

//=======================================
struct sockaddr_in gcAddr; 
struct sockaddr_in locAddr;
char * target_ip = "192.168.42.129"; //QGC addr

 
//======================================= 

int main()
{
    pid_t pid;
    pid_t pid1;

	if (init(target_ip, &gcAddr, &locAddr) != 0) {

		//TODO print some error info.
		return 1;	   
	}

	//for fork child no hangup 
	if (signal(SIGCHLD,SIG_IGN) == SIG_ERR) {
		perror("signal error");
		exit(EXIT_FAILURE);
	}
	
	//TODO fork maybe 	
	if ((pid = fork()) < 0) { // 创建进程
		perror("fork");  
		exit(EXIT_FAILURE);	  
	}

	if (pid == 0) { // 创建子进程成功

		if ((pid1 = fork()) < 0) { // 创建进程
			perror("fork");  
			exit(EXIT_FAILURE);   
		}
			  
		if (pid1 == 0) { //创建子进程成功
			printf("apple:task 2 will be running \n");
			system("br0up");
			
			while(1) {
				task2_videdata();
				sleep(3);
			}
			
			printf("task 2 over \n");
			exit(0);
			
		} else {         //创建父进程成功
			printf("apple:task 3 will be running \n");
			
			while(1) {
				task3_MavPddl();
				sleep(2);
			}
			
			printf("task 3 over \n");
			exit(0);
		}
		
	} else { // 创建父进程成功
		printf("apple:task 1 will be running \n");
		
		while(1) {
			task1_MavData(); 
			// sleep(1);
		}
		
		printf("task 1 over \n");
		exit(0);
	}

}

