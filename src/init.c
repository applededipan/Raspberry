#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

//=============== net ==========================
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
#include "wiringSerial.h"
#include "init.h"

int fd1; //fd for serial task1
int udpsock;

bool init(char *ip, struct sockaddr_in *a, struct sockaddr_in *b)
{
    udpsock = udpinit(ip,a,b);
  
    if ((fd1 = serialOpen ("/dev/ttyS0", 115200)) < 0) {
        fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno));
        return false;
    } 

    //someelse will add following ...

    return true;

}
