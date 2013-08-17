#!/bin/bash
#/etc/init.d/automatedsender.sh
#this automates the sending of the current temperature and uptime of the raspberry pi

while [ true ]; do
	TEMPC=`(echo -n "$(/home/pi/bin/measuretemp.sh)")`
	TIME=`(cat /proc/uptime | cut -d " " -f1)`
	USEDMEM=`(echo -n "$(/home/pi/bin/usedmem.sh)")`
	USEDCPU=`(echo -n "$(/home/pi/bin/usedcpu.sh)")`
	DELIM=' '
	PACKET="$TEMPC"
	PACKET+="$DELIM"
	PACKET+="$TIME"
	PACKET+="$DELIM"
	PACKET+="$USEDMEM"
	PACKET+="$DELIM"
	PACKET+="$USEDCPU"
	#echo "$PACKET"
	/home/pi/bin/sendudp.sh "$PACKET"

	sleep 0.5
done
