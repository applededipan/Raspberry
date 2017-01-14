#!/bin/bash
exec 2> /home/pi/log/erro.log
exec 1> /home/pi/log/info.log
function br0down 
{
    ifconfig br0 down
    brctl delbr br0
 }
  
  if ifconfig | grep "br0"
  	then
  	   if br0down
          then
	           echo br0down has done
       else
	           echo br0down error
      fi
  fi



  
