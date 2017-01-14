#!/bin/bash
ping -c 2 192.168.42.123 | grep ttl > /dev/null
echo $?
