#!/bin/bash

echo -e "\e[96mFlashing multiple Krembots started..."
tput sgr0
while read p; do	
./flash_photon.sh $p $2 &
tput sgr0
sleep 0.2
done < $1
wait
