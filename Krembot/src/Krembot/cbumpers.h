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

/* Author: Elhay Rauper, Yair Shlomi */


#ifndef CBUMPERS_H
#define CBUMPERS_H

#include <Wire.h> //i2c
#include "SparkFunSX1509/SparkFunSX1509.h" //mux
#include "version_detector.h"

#define MUX_ADDR 0x3E
#define BUMPER_FRONT 7
#define BUMPER_FRONT_RIGHT 0
#define BUMPER_RIGHT 1
#define BUMPER_REAR_RIGHT 2
#define BUMPER_REAR 3
#define BUMPER_REAR_LEFT 4
#define BUMPER_LEFT 5
#define BUMPER_FRONT_LEFT 6

enum BumperState
{
	UNDEFINED,
	PRESSED,
	UNPRESSED
};


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


struct BumpersRes
{
	BumperState front = BumperState::UNDEFINED,
							front_right = BumperState::UNDEFINED,
							right = BumperState::UNDEFINED,
							rear_right = BumperState::UNDEFINED,
							rear = BumperState::UNDEFINED,
							rear_left = BumperState::UNDEFINED,
							left = BumperState::UNDEFINED,
							front_left = BumperState::UNDEFINED;

	bool isAnyPressed()
	{
		if(front == BumperState::PRESSED || front_right == BumperState::PRESSED || right == BumperState::PRESSED || rear_right == BumperState::PRESSED ||
		 rear == BumperState::PRESSED || rear_left == BumperState::PRESSED || left == BumperState::PRESSED || front_left == BumperState::PRESSED)
		{
			return true;
		}
			return false;
	}
};


class CBumpers
{

private:

	SX1509 mux_;
	uint8_t calib_stage_ = 0;
	BumpCalib bumps_calib_;
	Version version;

public:

	CBumpers();
	void init (Version hwVersion);
	BumpersRes read();
	void print();
	void publish();

	void printCalib();
 	bool calib();
	void countDown(uint8_t duration);
	uint16_t readRaw();

};

#endif
