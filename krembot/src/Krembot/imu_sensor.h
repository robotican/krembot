#ifndef IMUSENSOR_H
#define IMUSENSOR_H

//TODO: IMPLEMENT

#include "quaternionFilters.h"
#include "MPU9250.h"


#define AHRS true         // Set to false for basic data read
#define SerialDebug false  // Set to true to get Serial output for debugging

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
public:
  bool init();
  void read(ImuData &data);


};

#endif
