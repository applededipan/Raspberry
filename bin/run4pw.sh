#! /bin/bash
# to add bridge shell into the Path

# to run main


if [ -x /home/pi/Firmware/shell/br0up ]
then 
    echo that is ok
else
    sudo chmod 755 /home/pi/Firmware/shell/*
fi



if [ -f /usr/bin/sub-br0up.sh -a -f /usr/bin/br0up ]

then 
    echo do not need copy
else
    sudo cp /home/pi/Firmware/shell/* /usr/bin/
fi

# delete main_temp file
if [ -f /home/pi/Firmware/src/main_temp ]
	sudo rm -f /home/pi/Firmware/src/main_temp
fi

# check if new firmware exist
if [ -f /home/pi/Firmware/src/main_update ]
	sudo mv -f /home/pi/Firmware/src/main_update /home/pi/Firmware/src/main
fi

sudo chmod 755 /home/pi/Firmware/src/main
/home/pi/Firmware/src/main



# check if new firmware exist
#if [ -f /home/pi/Firmware/src/main_update ]
#then
#    sudo chmod 755 /home/pi/Firmware/src/main_update
#    /home/pi/Firmware/src/main_update
#else
#    sudo chmod 755 /home/pi/Firmware/src/main
#    /home/pi/Firmware/src/main
#fi


