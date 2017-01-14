#!/bin/bash
function br0up {
 brctl addbr br0 
 brctl addif br0 eth1 
 brctl addif br0 usb0 
 ifconfig br0 192.168.42.122 netmask 255.255.255.0 up 
 }

exec 2> /home/pi/log/erro.log
exec 1> /home/pi/log/info.log
echo br0up begine

if ifconfig | grep "br0"
then
	br0down
fi

if br0up 
then
	echo br0up done
else
	echo error
fi
