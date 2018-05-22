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

/* Author:  Elhay Rauper*/

#include "krembot.h"

void Krembot::setup()
{
	Serial.begin(38400);
	Particle.subscribe("spark/", &Krembot::saveMyName, this);
	Particle.subscribe("reset", &Krembot::reset, this);
	Particle.publish("spark/device/name");
	delay(2000); // give cloud time to register

	Wire.begin();

	//rgba & imu sensors can only be init after wire.begin
	imuInitErrors = imu.init();

	RgbaFront.init(uint8_t(RGBAAddr::Front));
	RgbaRear.init(uint8_t(RGBAAddr::Rear));

	RgbaFrontRight.init(uint8_t(RGBAAddr::FrontRight));
	RgbaRight.init(uint8_t(RGBAAddr::Right));
	RgbaRearRight.init(uint8_t(RGBAAddr::RearRight));
	RgbaRearLeft.init(uint8_t(RGBAAddr::RearLeft));
	RgbaLeft.init(uint8_t(RGBAAddr::Left));
	RgbaFrontLeft.init(uint8_t(RGBAAddr::FrontLeft));

	my_name_ = "";
}

void Krembot::loop()
{
	//imu_sensor.magCalLoop();
	imu.loop();
}

void Krembot::saveMyName(const char *topic, const char *data)
{
	Serial.println("[Krembot]: my name is " + String(data));
	my_name_ = String(data);
}
