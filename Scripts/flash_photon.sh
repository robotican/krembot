#!/bin/bash

echo -e "\e[34mFlashing $1: started"
tput sgr0
particle flash $1 $2
if [ $? -eq 0 ]; then 
	echo -e "\e[92mFlashing $1: success\e[49m"
else
	echo -e "\e[91mFlashing $1: failed\e[49m"
fi
tput sgr0

