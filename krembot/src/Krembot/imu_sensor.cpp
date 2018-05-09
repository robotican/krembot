#include "imu_sensor.h"

ImuInitErrors IMUSensor::init()
{
  // Set up the interrupt pin, its set as active high, push-pull
//  pinMode(intPin, INPUT);
//  digitalWrite(intPin, LOW);
  //pinMode(myLed, OUTPUT);
  //digitalWrite(myLed, HIGH);

  // Read the WHO_AM_I register, this is a good test of communication
  ImuInitErrors errors;
  byte c = imu_.readByte(MPU9250_ADDRESS, WHO_AM_I_MPU9250);
  //Serial.print("MPU9250 "); Serial.print("I AM "); Serial.print(c, HEX);
  //Serial.print(" I should be "); Serial.println(0x71, HEX);
  if(c == 0x71)
  {
    errors.imu_adrees_ok = true;
  }
  else Serial.println("imu_adrees_ok = false");

  if (c == 0x71) // WHO_AM_I should always be 0x68
  {
    //Serial.println("MPU9250 is online...");
    errors.imu_online = true;
    // Start by performing self test and reporting values
    imu_.MPU9250SelfTest(imu_.SelfTest);
    /*if(SerialDebug)
    {
      Serial.print("x-axis self test: acceleration trim within : ");
      Serial.print(imu_.SelfTest[0],1); Serial.println("% of factory value");
      Serial.print("y-axis self test: acceleration trim within : ");
      Serial.print(imu_.SelfTest[1],1); Serial.println("% of factory value");
      Serial.print("z-axis self test: acceleration trim within : ");
      Serial.print(imu_.SelfTest[2],1); Serial.println("% of factory value");
      Serial.print("x-axis self test: gyration trim within : ");
      Serial.print(imu_.SelfTest[3],1); Serial.println("% of factory value");
      Serial.print("y-axis self test: gyration trim within : ");
      Serial.print(imu_.SelfTest[4],1); Serial.println("% of factory value");
      Serial.print("z-axis self test: gyration trim within : ");
      Serial.print(imu_.SelfTest[5],1); Serial.println("% of factory value");
    }*/

    // Calibrate gyro and accelerometers, load biases in bias registers
    imu_.calibrateMPU9250(imu_.gyroBias, imu_.accelBias);

    imu_.initMPU9250();
    // Initialize device for active mode read of acclerometer, gyroscope, and
    // temperature
    //Serial.println("MPU9250 initialized for active data mode....");

    // Read the WHO_AM_I register of the magnetometer, this is a good test of
    // communication
    byte d = imu_.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    //Serial.print("AK8963 "); Serial.print("I AM "); Serial.print(d, HEX);
    //Serial.print(" I should be "); Serial.println(0x48, HEX);
    if(d == 0x48)
    {
      errors.mag_adrees_ok = true;
    }
    else Serial.println("mag_adrees_ok = false");

    // Get magnetometer calibration from AK8963 ROM

    imu_.initAK8963(imu_.magCalibration);
    EEPROM.get(MAG_BIAS_0_ADDR, mag_bias0);
    EEPROM.get(MAG_BIAS_1_ADDR, mag_bias1);
    EEPROM.get(MAG_BIAS_2_ADDR, mag_bias2);

    /*Serial.print("mag_bias0: ");
    Serial.println(mag_bias0);
    Serial.print("mag_bias1: ");
    Serial.println(mag_bias1);
    Serial.print("mag_bias2: ");
    Serial.println(mag_bias2);*/

    imu_.magbias[0] = (float)mag_bias0;
    imu_.magbias[1] = (float)mag_bias1;
    imu_.magbias[2] = (float)mag_bias2;



  /*  EEPROM.get(MAG_BIAS_0_ADDR, imu_.magbias[0]);
    EEPROM.get(MAG_BIAS_1_ADDR, imu_.magbias[1]);
    EEPROM.get(MAG_BIAS_2_ADDR, imu_.magbias[2]);*/

  /*  Serial.print("mag_bias0: ");
    Serial.println(imu_.magbias[0]);
    Serial.print("mag_bias1: ");
    Serial.println(imu_.magbias[1]);
    Serial.print("mag_bias2: ");
    Serial.println(imu_.magbias[2]);*/
    // Initialize device for active mode read of magnetometer
    //Serial.println("AK8963 initialized for active data mode....");
    /*if (SerialDebug)
    {
      //  Serial.println("Calibration values: ");
      Serial.print("X-Axis sensitivity adjustment value ");
      Serial.println(imu_.magCalibration[0], 2);
      Serial.print("Y-Axis sensitivity adjustment value ");
      Serial.println(imu_.magCalibration[1], 2);
      Serial.print("Z-Axis sensitivity adjustment value ");
      Serial.println(imu_.magCalibration[2], 2);
    }*/

  } // if (c == 0x71)
  else Serial.println("imu_online = false");
  return errors;

}

ImuData IMUSensor::read()
{
  ImuData data;

  data.roll = imu_.pitch;
  data.pitch = imu_.roll;
  data.yaw = imu_.yaw;

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
    imu_.ax = (float)imu_.accelCount[0]*imu_.aRes; // - accelBias[0];
    imu_.ay = (float)imu_.accelCount[1]*imu_.aRes; // - accelBias[1];
    imu_.az = (float)imu_.accelCount[2]*imu_.aRes; // - accelBias[2];

    imu_.readGyroData(imu_.gyroCount);  // Read the x/y/z adc values
    imu_.getGres();

    // Calculate the gyro value into actual degrees per second
    // This depends on scale being set
    imu_.gx = (float)imu_.gyroCount[0]*imu_.gRes;
    imu_.gy = (float)imu_.gyroCount[1]*imu_.gRes;
    imu_.gz = (float)imu_.gyroCount[2]*imu_.gRes;

    imu_.readMagData(imu_.magCount);  // Read the x/y/z adc values
    imu_.getMres();
    // User environmental x-axis correction in milliGauss, should be
    // automatically calculated


    imu_.magbias[0] = (float)mag_bias0;
    imu_.magbias[1] = (float)mag_bias1;
    imu_.magbias[2] = (float)mag_bias2;

    imu_.magbias[0] = +470.;
    // User environmental x-axis correction in milliGauss TODO axis??
    imu_.magbias[1] = +120.;
    // User environmental x-axis correction in milliGauss
    imu_.magbias[2] = +125.;

    // Calculate the magnetometer values in milliGauss
    // Include factory calibration per data sheet and user environmental
    // corrections
    // Get actual magnetometer value, this depends on scale being set
    imu_.mx = (float)imu_.magCount[0]*imu_.mRes*imu_.magCalibration[0] +
               imu_.magbias[0];
    imu_.my = (float)imu_.magCount[1]*imu_.mRes*imu_.magCalibration[1] +
               imu_.magbias[1];
    imu_.mz = (float)imu_.magCount[2]*imu_.mRes*imu_.magCalibration[2] +
               imu_.magbias[2];
  } // if (readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)

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
    //if(imu_.delt_t > 500)
    //{
      //if(SerialDebug)
      //{
        // Print acceleration values in milligs!
        /*Serial.print("X-acceleration: "); Serial.print(1000*imu_.ax);
        Serial.print(" mg ");
        Serial.print("Y-acceleration: "); Serial.print(1000*imu_.ay);
        Serial.print(" mg ");
        Serial.print("Z-acceleration: "); Serial.print(1000*imu_.az);
        Serial.println(" mg ");*/

        // Print gyro values in degree/sec
       /* Serial.print("X-gyro rate: "); Serial.print(imu_.gx, 3);
        Serial.print(" degrees/sec ");
        Serial.print("Y-gyro rate: "); Serial.print(imu_.gy, 3);
        Serial.print(" degrees/sec ");
        Serial.print("Z-gyro rate: "); Serial.print(imu_.gz, 3);
        Serial.println(" degrees/sec");*/

        // Print mag values in degree/sec
       /* Serial.print("X-mag field: "); Serial.print(imu_.mx);
        Serial.print(" mG ");
        Serial.print("Y-mag field: "); Serial.print(imu_.my);
        Serial.print(" mG ");
        Serial.print("Z-mag field: "); Serial.print(imu_.mz);
        Serial.println(" mG");*/

        //imu_.tempCount = imu_.readTempData();  // Read the adc values
        // Temperature in degrees Centigrade
        //imu_.temperature = ((float) imu_.tempCount) / 333.87 + 21.0;
        // Print temperature in degrees Centigrade
        /*Serial.print("Temperature is ");  Serial.print(imu_.temperature, 1);
        Serial.println(" degrees C");*/
      //}


      imu_.count = millis();
    //} // if (imu_.delt_t > 500)
  } // if (!AHRS)
  else
  {
    // Serial print and/or display at 0.5 s rate independent of data rates
    imu_.delt_t = millis() - imu_.count;

    // update LCD once per half-second independent of read rate
    //if (imu_.delt_t > 500)
    //{
      /*if(SerialDebug)
      {
        Serial.print("ax = "); Serial.print((int)1000*imu_.ax);
        Serial.print(" ay = "); Serial.print((int)1000*imu_.ay);
        Serial.print(" az = "); Serial.print((int)1000*imu_.az);
        Serial.println(" mg");

        Serial.print("gx = "); Serial.print( imu_.gx, 2);
        Serial.print(" gy = "); Serial.print( imu_.gy, 2);
        Serial.print(" gz = "); Serial.print( imu_.gz, 2);
        Serial.println(" deg/s");

        Serial.print("mx = "); Serial.print( (int)imu_.mx );
        Serial.print(" my = "); Serial.print( (int)imu_.my );
        Serial.print(" mz = "); Serial.print( (int)imu_.mz );
        Serial.println(" mG");

        Serial.print("q0 = "); Serial.print(*getQ());
        Serial.print(" qx = "); Serial.print(*(getQ() + 1));
        Serial.print(" qy = "); Serial.print(*(getQ() + 2));
        Serial.print(" qz = "); Serial.println(*(getQ() + 3));
      }*/

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
      imu_.yaw   = atan2(2.0f * (*(getQ()+1) * *(getQ()+2) + *getQ() *
                    *(getQ()+3)), *getQ() * *getQ() + *(getQ()+1) * *(getQ()+1)
                    - *(getQ()+2) * *(getQ()+2) - *(getQ()+3) * *(getQ()+3));
      imu_.pitch = -asin(2.0f * (*(getQ()+1) * *(getQ()+3) - *getQ() *
                    *(getQ()+2)));
      imu_.roll  = atan2(2.0f * (*getQ() * *(getQ()+1) + *(getQ()+2) *
                    *(getQ()+3)), *getQ() * *getQ() - *(getQ()+1) * *(getQ()+1)
                    - *(getQ()+2) * *(getQ()+2) + *(getQ()+3) * *(getQ()+3));
      imu_.pitch *= RAD_TO_DEG;
      imu_.yaw   *= RAD_TO_DEG;
      // Declination of SparkFun Electronics (40°05'26.6"N 105°11'05.9"W) is
      //  8° 30' E  ± 0° 21' (or 8.5°) on 2016-07-19
      // - http://www.ngdc.noaa.gov/geomag-web/#declination
      imu_.yaw   -= 8.5;
      imu_.roll  *= RAD_TO_DEG;



      //if(SerialDebug)
      //{
       /* Serial.print("Roll, Pitch, Yaw: ");
        Serial.print(imu_.roll, 2);
        Serial.print(", ");
        Serial.print(imu_.pitch, 2);
        Serial.print(", ");
        Serial.println(imu_.yaw, 2);


        Serial.print("rate = ");
        Serial.print((float)imu_.sumCount/imu_.sum, 2);
        Serial.println(" Hz");*/
      //}

      imu_.count = millis();
      imu_.sumCount = 0;
      imu_.sum = 0;
    //} // if (imu_.delt_t > 500)
  } // if (AHRS)
}

void IMUSensor::print()
{
  Serial.print("Roll, Pitch, Yaw: ");
  Serial.print(imu_.pitch, 2);
  Serial.print(", ");
  Serial.print(imu_.roll, 2);
  Serial.print(", ");
  Serial.println(imu_.yaw, 2);
}
