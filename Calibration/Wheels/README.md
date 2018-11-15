# Wheels calibration


If a krembot not driving in straight line when the propper command is received the wheels may need calibration.
* If the krembot is turning right insterd of driving in a straight line, we will add a little offset to the left. 
* If the krembot is turning left insterd of driving in a straight line, we will add a little offset to the right.

1. To start the calibration process flash the calibration sketch to the krembot.
2. Go to your particle [events control panel](https://console.particle.io/events)
3. Choose the krembot you want to calibrate (make sure it's online).

4. Press on the green Paper plane (Publish Event).

5. Fill the "Event Name" box with "wheels". In the "Event Data" box, write one of the following command:

	* go - the krembot will drive forward for 5 seconds. the red leds will turn on for 5 seconds.
	* back - the krembot will drive backward for 5 seconds. the red leds will turn on for 5 seconds.
	* left_num - add offset to the left wheel. replace num with a number, e.g left 3. the blue leds will turn on for 3 seconds
	* right_num - add offset to the right wheel. replace num with a number, e.g right 3. the blue leds will turn on for 3 seconds
	* save - save the offsets in the krembots memory. the green leds will turn on for 4 seconds. after saving the offsets, the save command is disabled until the releas command is entered.
	* 123 - re enable the save command.

* There are some limitations - 
	* Adding offset to a wheel will override the previous value.
	* re enabling the save command will work only if you wait 30 seconds after saving.
	* somtimes we will need to wait a few seconds until the krembot will execute the command we sent. please wait patiently.
