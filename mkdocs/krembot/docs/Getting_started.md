#Getting started

##Basic Code Pattern
KREMEBOt API was designed to be as simple and easy to use as possible. The most basic code required to get start coding includes the following:

1. KREMEBOt header file (krembot.h)
2. Single KREMEBOt object instance
3. KREMEBOt's setup() function
4. KREMEBOt's loop() function

See example [here](https://github.com/robotican/krembot_1.0/blob/master/krembot/src/krembot_main.ino)

##Compiling ang flashing code for KREMEBOt

Use the particle-dev to select the target KREMEBOt and compile and flash using the cloud.

See:

[Compiling](https://docs.particle.io/guide/tools-and-features/dev/#compiling-code)

[Flashing](https://docs.particle.io/guide/tools-and-features/dev/#flashing-device)

#Flashing multiple KREMEBOts

Compile your code in particle-dev (without flashing). This step will create a compiled bin file inside ~/KREMEBOt folder.

Create a group file with the names of all the KREMEBOts you want to flash (each name in a new line).

    $ cd ~/krembot/flash
    $ ./flash_multi.sh <group file path> <bin file path>

For example:

    $ ./flash_multi.sh group_all.txt ../krembot/photon_0.6.2_firmware_1499256083785.bin

#More important things you need to know:
* **Do not** use delay() or long for or while loops. use Timers (e.g. SandTime class) insted.
* Turn on the KREMEBOt only on flat surface.
* If the battery goes below 10% it can cause unknown behavior.   
* If the battery will drain completely, the red lights will be turn on for 30 seconds (during this time the KREMEBOt will stop executing any other code).
After this 30 seconds the KREMEBOt will go to sleep for a minute.  
Next time the KREMEBOt will be turned on, this 30 seconds of red light and 1 minute of sleep will happen again unless the KREMEBOt will be charged.
* before using any KREMEBOt it is **highly recommended** to flash an empty sketch to it, to prevent any unpredicted movment (from previously flashed sketches).  
* when charging your KREMEBOt, turn off the KREMEBOt for faster charging.
