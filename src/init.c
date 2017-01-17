

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
    creatPingTarget();
    return true;

}
