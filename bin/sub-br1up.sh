#!/bin/bash
exec 2> /home/pi/log/erro.log
exec 1> /home/pi/log/info.log
function br1up {
 brctl addbr br1
 brctl addif br1 eth1
 brctl addif br1 eth0
 ifconfig br1 192.168.42.122 netmask 255.255.255.0 up
 }

if ifconfig | grep "br1"
then
	br1down
fi

 if br1up
 then
	 echo br1up has done
 else
	 echo br1up erro
 fi
