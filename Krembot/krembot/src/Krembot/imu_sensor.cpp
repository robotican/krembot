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


#include "imu_sensor.h"

void IMUSensor::init()
{
	// Read the WHO_AM_I register, this is a good test of communication
	byte c = imu_.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);

	if (c == 0x71)
	{
		InitFlags.address = true;
		Serial.println("[IMU]: online");
		InitFlags.online = true;

		// Start by performing self test and reporting values
		imu_.MPU9250SelfTest(imu_.SelfTest);
		delay(300);
		// Calibrate gyro and accelerometers, load biases in bias registers
		imu_.calibrateMPU9250(imu_.gyroBias, imu_.accelBias);

		// Initialize device for active mode read of acclerometer, gyroscope, and
		// temperature
		imu_.initMPU9250();
		// Read the WHO_AM_I register of the magnetometer, this is a good test of
		// communication
	}
	else
	{
		Serial.println("[IMU]: failed to communicate");
	}
}


// get latest IMU fused data
ImuData IMUSensor::read()
{
	ImuData data;

	// assing readings to the correct real world axes
	data.roll = imu_.pitch;
	data.pitch = imu_.roll;
	data.yaw = imu_.yaw;

	data.ax = imu_.ax;
	data.ax = imu_.ay;
	data.ax = imu_.az;

	data.ax = imu_.gx;
	data.ax = imu_.gy;
	data.ax = imu_.gz;

	return data;
}


void IMUSensor::loop()
{
	// If intPin goes high, all data registers have new data
	// On interrupt, check if data ready interrupt
	if (imu_.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
	{
		imu_.readAccelData(imu_.accelCount);  // Read the x/y/z adc values
		imu_.getAres();

		// Now we'll calculate the accleration value into actual g's
		// This depends on scale being set
		imu_.ax = (float)imu_.accelCount[0] * imu_.aRes;
		imu_.ay = (float)imu_.accelCount[1] * imu_.aRes;
		imu_.az = (float)imu_.accelCount[2] * imu_.aRes;

		imu_.readGyroData(imu_.gyroCount);  // Read the x/y/z adc values
		imu_.getGres();

		// Calculate the gyro value into actual degrees per second
		// This depends on scale being set
		imu_.gx = (float)imu_.gyroCount[0] * imu_.gRes;
		imu_.gy = (float)imu_.gyroCount[1] * imu_.gRes;
		imu_.gz = (float)imu_.gyroCount[2] * imu_.gRes;


	}

	// Must be called before updating quaternions!
	imu_.updateTime();
	if (abs(imu_.gz) < 0.2)
	{
		imu_.gz = 0;
	}

	yaw += imu_.gz * imu_.deltat;
	pitch = -atan2(-imu_.ax , imu_.az) * RAD_TO_DEG;
	roll = -atan2( imu_.ay , sqrt(imu_.az * imu_.az + imu_.ax * imu_.ax)) * RAD_TO_DEG;

	imu_.yaw = yaw * (-1);
	imu_.roll = roll;
	imu_.pitch = pitch;
}

void IMUSensor::print()
{
	Serial.print("[IMU]: Roll, Pitch, Yaw");
	Serial.print(imu_.roll, 2);
	Serial.print(", ");
	Serial.print(imu_.pitch, 2);
	Serial.print(", ");
	Serial.println(imu_.yaw, 2);
}


void IMUSensor::printRaw()
{
	print();

	Serial.print("ax: ");Serial.print(imu_.ax);
	Serial.print(", ay: ");Serial.print(imu_.ay);
	Serial.print(", az: ");Serial.println(imu_.az);
	Serial.print(" gx: ");Serial.print(imu_.gx);
	Serial.print(", gy: ");Serial.print(imu_.gy);
	Serial.print(", gz: ");Serial.println(imu_.gz);

}

void IMUSensor::publish()
{
	String publishStr = "[IMU]: Roll, Pitch, Yaw";
	String roll = String (imu_.roll, 2);
	publishStr.concat(roll);
	publishStr.concat(", ");
	String pitch = String (imu_.pitch, 2);
	publishStr.concat(pitch);
	publishStr.concat(", ");
	String yaw = String (imu_.yaw, 2);
	publishStr.concat(yaw);

	Particle.publish("Imu", publishStr, PRIVATE);

}
