# flash scripts

this scripts was tested on ubuntu 16.04 and will not work on windows.

* Compile your code in particle workbench (without flashing). This step will create a compiled bin file inside src folder.
* Create a group file with the names of all the KREMEBOts you want to flash (each name in a new line).  
* run the following command (the script files are located inside Scripts folder) 
$ ./flash_multi.sh <group file path> <bin file path>

For example:

$ ./flash_multi.sh group_all.txt ../krembot/photon_0.6.2_firmware_1499256083785.bin

** The names in the group file must be identical to the krembots names. check for capital letters mismatch**
