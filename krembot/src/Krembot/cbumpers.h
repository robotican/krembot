/*******************************************************************************
* Copyright (c) 2018, RoboTICan, LTD.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright notice, this
*   list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright notice,
*   this list of conditions and the following disclaimer in the documentation
*   and/or other materials provided with the distribution.
*
* * Neither the name of RoboTICan nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/* Author: Elhay Rauper */
/* Maintainer: Yair Shlomi */


#ifndef CBUMPERS_H
#define CBUMPERS_H

#include <Wire.h> //i2c
#include "SparkFunSX1509/SparkFunSX1509.h" //mux
#include "hardware_selector.h"

#ifdef krembot_V2

#define MUX_ADDR 0x3E
#define BUMPER_FRONT 7
#define BUMPER_FRONT_RIGHT 0
#define BUMPER_RIGHT 1
#define BUMPER_REAR_RIGHT 2
#define BUMPER_REAR 3
#define BUMPER_REAR_LEFT 4
#define BUMPER_LEFT 5
#define BUMPER_FRONT_LEFT 6

#endif

#ifdef krembot_V1
	#define BUMPERS_LEG A0
	#define TOLERANCE 10
	#define CALIB_TIMEOUT 5

	#define NO_BUMP 0


	struct BumpCalib
	{
	  uint16_t left,
	          right,
	          right_left,
	          rear,
	          rear_left,
	          rear_right,
	          rear_left_right,
	          front,
	          front_left,
	          front_right,
	          front_right_left,
	          front_rear,
	          front_rear_left,
	          front_rear_right,
	          front_rear_right_left;
	};
#endif


struct BumpersRes
{
#ifdef krembot_V2
	bool front_right = false,
		rear_right = false,
		rear_left = false,
		front_left = false;
#endif
	bool front = false,
		right = false,
		rear = false,
		left = false;


		bool isAnyPressed()
		{
			if(front || right || rear || left)
			{
				return true;
			}

#ifdef krembot_V2
			if(front_right || rear_right || rear_left || front_left)
			{
				return true;
			}
#endif
			return false;
		}
};


class CBumpers
{

private:

#ifdef krembot_V2
	SX1509 mux_;
#endif

#ifdef krembot_V1
	uint8_t calib_stage_ = 0;
	BumpCalib bumps_calib_;
#endif

public:

	CBumpers();
	BumpersRes read();
	void print();
	void publish();

#ifdef krembot_V1
	void printCalib();
 	bool calib();
	void countDown(uint8_t duration);
	uint16_t readRaw();
#endif

};

#endif
