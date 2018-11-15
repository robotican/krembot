/*******************************************************************************
* Copyright (c) 2018 RoboTICan
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the disclaimer
* below) provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*
*     * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*
*     * Neither the name of the copyright holder nor the names of its
*     contributors may be used to endorse or promote products derived from this
*     software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
* THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
* IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/* Author: Yair Shlomi */

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
