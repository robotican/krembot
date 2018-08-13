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

ImuInitErrors IMUSensor::init()
{
	ImuInitErrors errors;
	// Read the WHO_AM_I register, this is a good test of communication
	byte c = imu_.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);

	if (c == 0x71)
	{
		errors.imu_address_ok = true;
		Serial.println("[IMU]: online");
		errors.imu_online = true;

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

		byte d = imu_.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
		if (d == 0x48)
		{
			Serial.println("[IMU]: magnetometer online");
			errors.mag_address_ok = true;

			// Get magnetometer calibration from AK8963 ROM
			/*imu_.initAK8963(imu_.magCalibration);

			// try to get magnetometer calibration data from EEPROM
			EEPROM.get(MAG_CAL_ADDR, mag_bias);
			if (mag_bias.id == IMUSensor::MAG_BIAS_ID)
			{
				Serial.println("[IMU]: found magnetometer calibration data in EEPROM:");

			}
			else
			{
				Serial.println("[IMU]: didn't found magnetometer calibration data in EEPROM. Setting bias to defaults");
				mag_bias.x = 470;
				mag_bias.y = 120;
				mag_bias.z = 125;


				// in case no calibration was found, init bias arrays with mag raw readings
				// to prepare for calibration
				imu_.readMagData(mag_temp);
				for (int jj = 0; jj < 3; jj++)
				{
					mag_max[jj] = mag_temp[jj];
					mag_min[jj] = mag_temp[jj];
				}
			}*/
		}
		else
			Serial.println("[IMU]: failed to communicate with magnetometer");
	}
	else
	{
		Serial.println("[IMU]: failed to communicate");
	}

	return errors;
}

void IMUSensor::resetMagCal()
{
	mag_bias.x = 0;
	mag_bias.y = 0;
	mag_bias.z = 0;
	mag_bias.id = IMUSensor::MAG_BIAS_ID;
	EEPROM.put(MAG_CAL_ADDR, mag_bias);
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

// for internal use: use inside krembot.loop to calibrate imu magnetometer
void IMUSensor::magCalLoop()
{
	Serial.println("[IMU]: Mag Calibration: Wave device in a figure eight. Press 's' to save calibration");

	char c = '\0';
	if (Serial.available() > 0)
		c = Serial.read();

	// press 's' to save calibration to EEPROM
	if (c != 's')
	{
		imu_.readMagData(mag_temp);  // Read the mag data
		for (int jj = 0; jj < 3; jj++)
		{

			if (mag_temp[jj] > mag_max[jj])
				mag_max[jj] = mag_temp[jj];

			if (mag_temp[jj] < mag_min[jj])
				mag_min[jj] = mag_temp[jj];

			Serial.print("mag_temp[jj]:");
			Serial.print(mag_temp[jj]);
			Serial.print(" | ");
			Serial.print("mag_max[jj]:");
			Serial.print(mag_max[jj]);
			Serial.print(" | ");
			Serial.print("mag_min[jj]:");
			Serial.print(mag_min[jj]);
			Serial.println();
		}
		delay(135);  // at 8 Hz ODR, new mag data is available every 125 ms
	}
	else
	{
		mag_bias.x = (mag_max[0] + mag_min[0]) / 2;  // get average x mag bias in counts
		mag_bias.y = (mag_max[1] + mag_min[1]) / 2;  // get average y mag bias in counts
		mag_bias.z = (mag_max[2] + mag_min[2]) / 2;  // get average z mag bias in counts

		Serial.println("[IMU]: Saving calibration with the following data:");
		Serial.print("mag_bias.x:");
		Serial.print(mag_bias.x);
		Serial.print(" | ");
		Serial.print("mag_bias.y:");
		Serial.print(mag_bias.y);
		Serial.print(" | ");
		Serial.print("mag_bias.z:");
		Serial.print(mag_bias.z);
		Serial.println();

		// save calibration
		mag_bias.id = IMUSensor::MAG_BIAS_ID;
		EEPROM.put(MAG_CAL_ADDR, mag_bias);
		Serial.print("***** calibration done. please upload other sketch *****");
		calibrationDone = true;
	}
}

double yaw=0,roll=0,pitch=0;
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
if (abs(imu_.gz)<0.2) imu_.gz=0;

yaw+=imu_.gz*imu_.deltat;
pitch = -atan2(-imu_.ax , imu_.az) * RAD_TO_DEG;
roll = -atan2( imu_.ay , sqrt(imu_.az * imu_.az + imu_.ax * imu_.ax)) * RAD_TO_DEG;

		imu_.yaw=yaw;

		imu_.roll =roll;
imu_.pitch=pitch;

		//print();

}

void IMUSensor::print()
{
	Serial.print("[IMU]: Roll, Pitch, Yaw, Yae rate: ");
	Serial.print(imu_.pitch, 2);
	Serial.print(", ");
	Serial.print(imu_.roll, 2);
	Serial.print(", ");
	Serial.print(imu_.yaw, 2);
	Serial.print(", ");
	Serial.println(imu_.gz, 2);
}

void IMUSensor::printRaw()
{
	print();

	Serial.print("ax: ");Serial.print(imu_.ax);
	Serial.print(", ay: ");Serial.print(imu_.ay);
	Serial.print(", az: ");Serial.println(imu_.az);
	Serial.print("gx: ");Serial.print(imu_.gx);
	Serial.print(", gy: ");Serial.print(imu_.gy);
	Serial.print(", gz: ");Serial.println(imu_.gz);
	//Serial.print("mx: ");Serial.print(imu_.mx);
	//Serial.print(", my: ");Serial.print(imu_.my);
	//Serial.print(", mz: ");Serial.println(imu_.mz);

}
