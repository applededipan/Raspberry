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
then
	sudo rm -f /home/pi/Firmware/src/main_temp
	echo "apple: delete main_temp file"
fi

# check if new firmware exist and then rename
if [ -f /home/pi/Firmware/src/main_update ]
then
	sudo mv -f /home/pi/Firmware/src/main_update /home/pi/Firmware/src/main
	echo "apple: rename main_update file"
fi

sudo chmod 755 /home/pi/Firmware/src/main
/home/pi/Firmware/src/main
echo "apple: main running!"



# check if new firmware exist
#if [ -f /home/pi/Firmware/src/main_update ]
#then
#    sudo chmod 755 /home/pi/Firmware/src/main_update
#    /home/pi/Firmware/src/main_update
#else
#    sudo chmod 755 /home/pi/Firmware/src/main
#    /home/pi/Firmware/src/main
#fi


