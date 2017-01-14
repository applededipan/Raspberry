#!/bin/bash
exec 2> /home/pi/log/erro.log
exec 1> /home/pi/log/info.log
function br1down
{
  ifconfig br1 down
  brctl delbr br1
  }

  if ifconfig | grep "br1"
  	then
  	   if br1down
          then
	           echo br0down has done
       else
	           echo br0down error
      fi
  fi




