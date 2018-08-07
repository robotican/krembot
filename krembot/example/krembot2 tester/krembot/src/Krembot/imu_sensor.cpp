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
			imu_.initAK8963(imu_.magCalibration);

			// try to get magnetometer calibration data from EEPROM
			EEPROM.get(MAG_CAL_ADDR, mag_bias);
			if (mag_bias.id == IMUSensor::MAG_BIAS_ID)
			{
				Serial.println("[IMU]: found magnetometer calibration data in EEPROM:");
				/*Serial.print("mag_bias.x:");
				Serial.print(mag_bias.x);
				Serial.print(" | ");
				Serial.print("mag_bias.y:");
				Serial.print(mag_bias.y);
				Serial.print(" | ");
				Serial.print("mag_bias.z:");
				Serial.print(mag_bias.z);
				Serial.println();*/
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
			}
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

	data.ax = imu_.mx;
	data.ax = imu_.my;
	data.ax = imu_.mz;

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
	}
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

		imu_.readMagData(imu_.magCount);  // Read the x/y/z adc values
		imu_.getMres();

		// use magnetometer calibration data
		imu_.magbias[0] = (float)mag_bias.x*imu_.mRes*imu_.magCalibration[0];
		imu_.magbias[1] = (float)mag_bias.y*imu_.mRes*imu_.magCalibration[1];
		imu_.magbias[2] = (float)mag_bias.z*imu_.mRes*imu_.magCalibration[2];

		// Calculate the magnetometer values in milliGauss
		// Include factory calibration per data sheet and user environmental
		// corrections
		// Get actual magnetometer value, this depends on scale being set
		imu_.mx = (float)imu_.magCount[0] * imu_.mRes*imu_.magCalibration[0] -
			imu_.magbias[0];
		imu_.my = (float)imu_.magCount[1] * imu_.mRes*imu_.magCalibration[1] -
			imu_.magbias[1];
		imu_.mz = (float)imu_.magCount[2] * imu_.mRes*imu_.magCalibration[2] -
			imu_.magbias[2];
	}

	// Must be called before updating quaternions!
	imu_.updateTime();

	// Sensors x (y)-axis of the accelerometer is aligned with the y (x)-axis of
	// the magnetometer; the magnetometer z-axis (+ down) is opposite to z-axis
	// (+ up) of accelerometer and gyro! We have to make some allowance for this
	// orientationmismatch in feeding the output to the quaternion filter. For the
	// MPU-9250, we have chosen a magnetic rotation that keeps the sensor forward
	// along the x-axis just like in the LSM9DS0 sensor. This rotation can be
	// modified to allow any convenient orientation convention. This is ok by
	// aircraft orientation standards! Pass gyro rate as rad/s
  //  MadgwickQuaternionUpdate(ax, ay, az, gx*PI/180.0f, gy*PI/180.0f, gz*PI/180.0f,  my,  mx, mz);
	MahonyQuaternionUpdate(imu_.ax, imu_.ay, imu_.az, imu_.gx*DEG_TO_RAD,
		imu_.gy*DEG_TO_RAD, imu_.gz*DEG_TO_RAD, imu_.my,
		imu_.mx, imu_.mz, imu_.deltat);


	/*   Serial.print("rate = ");
	   Serial.print((float)imu_.sumCount/imu_.sum, 2);
	   Serial.println(" Hz");*/
	if (!AHRS)
	{
		imu_.delt_t = millis() - imu_.count;
		imu_.count = millis();
	} // if (!AHRS)
	else
	{
		// Serial print and/or display at 0.5 s rate independent of data rates
		imu_.delt_t = millis() - imu_.count;

		// Define output variables from updated quaternion---these are Tait-Bryan
		// angles, commonly used in aircraft orientation. In this coordinate system,
		// the positive z-axis is down toward Earth. Yaw is the angle between Sensor
		// x-axis and Earth magnetic North (or true North if corrected for local
		// declination, looking down on the sensor positive yaw is counterclockwise.
		// Pitch is angle between sensor x-axis and Earth ground plane, toward the
		// Earth is positive, up toward the sky is negative. Roll is angle between
		// sensor y-axis and Earth ground plane, y-axis up is positive roll. These
		// arise from the definition of the homogeneous rotation matrix constructed
		// from quaternions. Tait-Bryan angles as well as Euler angles are
		// non-commutative; that is, the get the correct orientation the rotations
		// must be applied in the correct order which for this configuration is yaw,
		// pitch, and then roll.
		// For more see
		// http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
		// which has additional links.
		imu_.yaw = atan2(2.0f * (*(getQ() + 1) * *(getQ() + 2) + *getQ() *
			*(getQ() + 3)), *getQ() * *getQ() + *(getQ() + 1) * *(getQ() + 1)
			- *(getQ() + 2) * *(getQ() + 2) - *(getQ() + 3) * *(getQ() + 3));
		imu_.pitch = -asin(2.0f * (*(getQ() + 1) * *(getQ() + 3) - *getQ() *
			*(getQ() + 2)));
		imu_.roll = atan2(2.0f * (*getQ() * *(getQ() + 1) + *(getQ() + 2) *
			*(getQ() + 3)), *getQ() * *getQ() - *(getQ() + 1) * *(getQ() + 1)
			- *(getQ() + 2) * *(getQ() + 2) + *(getQ() + 3) * *(getQ() + 3));
		imu_.pitch *= RAD_TO_DEG;
		imu_.yaw *= RAD_TO_DEG;

		// Declination of SparkFun Electronics (40°05'26.6"N 105°11'05.9"W) is
		//  8° 30' E  ± 0° 21' (or 8.5°) on 2016-07-19
		// - http://www.ngdc.noaa.gov/geomag-web/#declination
		imu_.yaw -= 4.6;

		imu_.yaw  += 90;
		if(imu_.yaw >= 180)
		{
			imu_.yaw -= 360;
		}
		if(imu_.yaw <= -180)
		{
			imu_.yaw += 360;
		}

		imu_.roll *= RAD_TO_DEG;

		imu_.count = millis();
		imu_.sumCount = 0;
		imu_.sum = 0;
	}
}

void IMUSensor::print()
{
	Serial.print("[IMU]: Roll, Pitch, Yaw: ");
	Serial.print(imu_.pitch, 2);
	Serial.print(", ");
	Serial.print(imu_.roll, 2);
	Serial.print(", ");
	Serial.println(imu_.yaw, 2);

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
	Serial.print("mx: ");Serial.print(imu_.mx);
	Serial.print(", my: ");Serial.print(imu_.my);
	Serial.print(", mz: ");Serial.println(imu_.mz);

}
