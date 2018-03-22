#ifndef RGBA_SENSOR_H
#define RGBA_SENSOR_H

#include "application.h"
#include "math.h"
#include "SparkFun_APDS9960.h"
#define MUX_ADDR 0x70


/**
* @brief Holds the values related to the color sensor
* 
*/
struct RGBAResult
{
  uint16_t Ambient;   /**< The Ambience of the light, (0-65535)*/
  uint16_t Red;       /**< The Red value of the light, (0-65535)*/
  uint16_t Green;     /**< The Green value of the light, (0-65535)*/
  uint16_t Blue;      /**< The Blue value of the light, (0-65535)*/
  uint8_t Proximity;  /**< The Raw value of the proximity sensor, (0-255)*/
  float Distance;     /**< The calculated distance to the object, in cm*/
  uint8_t ErrCode;    /**< The error code returned by the read function*/
  bool IsReadOk;      /**<  True if the reading was successful, false otherwise*/
};


class RGBASensor
{
private:

  uint8_t addr_;

  SparkFun_APDS9960 apds_;
  bool i2cMuxSelectMe();


public:

  
  
  void init(uint8_t addr);  

  /** 
  *   @brief  Reads the values from sensor.
  *   
  *   @return RGBAResult, a struct that holds the values of the sensor
  */    

  RGBAResult read();

  /** 
  *   @brief  Prints the values of the sensor.
  *   
  *   @return void
  */    
  void print();
};


#endif
