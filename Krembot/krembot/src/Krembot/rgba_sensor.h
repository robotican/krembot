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


#ifndef RGBA_SENSOR_H
#define RGBA_SENSOR_H

#include "application.h"
#include "math.h"
#include "SparkFun_APDS9960/SparkFun_APDS9960.h"
#define RGBA_MUX_ADDR 0x70

enum class RGBAAddr
{
  Front = 0,
  FrontRight = 1,
  Right = 2,
  RearRight = 3,
  Rear = 4,
  RearLeft = 5,
  Left = 6,
  FrontLeft = 7
};

struct RGBAResult
{
	uint16_t Ambient = 0;   /**< The Ambience of the light, (0-65535)*/
	uint16_t Red = 0;       /**< The Red value of the light, (0-65535)*/
	uint16_t Green = 0;     /**< The Green value of the light, (0-65535)*/
	uint16_t Blue = 0;      /**< The Blue value of the light, (0-65535)*/
	uint8_t Proximity = 0;  /**< The Raw value of the proximity sensor, (0-255)*/
	float Distance = 0;     /**< The calculated distance to the object, in cm*/
	bool AmbientError = false;
	bool RedError = false;
	bool GreenError = false;
	bool BlueError = false;
	bool ProximityError = false;
};

struct HSVResult
{
	double H, S, V;
};

enum Colors
{
  Red,
  Green,
  Blue,
  None
};

class RGBASensor
{
private:

	uint8_t addr_;
	String name_;
	SparkFun_APDS9960 apds_;
	bool i2cMuxSelectMe();
	void setName();

public:
	void init(uint8_t addr);
	RGBAResult readRGBA();
	HSVResult readHSV();
	Colors readColor();
	static HSVResult rgbToHSV(RGBAResult in);

	void printRGBA();
	void printHSV();
	void printColor();
	void print();

	void publish();

};


#endif
