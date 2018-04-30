#ifndef IMUSENSOR_H
#define IMUSENSOR_H

//TODO: IMPLEMENT

#include "quaternionFilters.h"
#include "MPU9250.h"
#include "application.h"

#define MAG_BIAS_0_ADDR 20
#define MAG_BIAS_1_ADDR 30
#define MAG_BIAS_2_ADDR 40


#define AHRS true         // Set to false for basic data read
#define SerialDebug false  // Set to true to get Serial output for debugging

struct ImuInitErrors
{
    bool imu_adrees_ok = false,
    	 imu_online = false,
    	 mag_adrees_ok = false;
};

struct ImuData
{
    float roll = 0,
          pitch = 0,
          yaw = 0;
};


class IMUSensor
{
private:
  //int intPin = 12;
  //int myLed = 13;
  MPU9250 imu_;
  int mag_bias0, mag_bias1, mag_bias2;

public:
  ImuInitErrors init();
  void loop();
  ImuData read();
  void print();


};

#endif
