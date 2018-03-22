
#include "rgba_sensor.h"
/** 
  *   @brief 
  *  
  *   @param 
  *   
  *   @return 
  *
  */

/** 
  *   @brief  Initializes the sensor.
  *  
  *   @param  addr is a uint8_t containing the address of the sensor (I2C protocol address)
  *   
  *   @return void
  */    

void RGBASensor::init(uint8_t addr)
{
  addr_ = addr;
  apds_ = SparkFun_APDS9960();
  i2cMuxSelectMe();

  if ( apds_.init() )
    Serial.print(F("APDS-9960 initialization complete sensor") );
  else
    Serial.println(F("Something went wrong during APDS-9960 init sensor" ));

  if ( apds_.enableLightSensor(false) )
    Serial.println(F("Light sensor is now running sensor" ));
  else
    Serial.println(F("Something went wrong during light sensor init! sensor" ));

  if ( !apds_.setProximityGain(PGAIN_2X) )
    Serial.println(F("Something went wrong trying to set PGAIN sensor sensor" ));

  if ( apds_.enableProximitySensor(false) )
    Serial.println(F("Proximity sensor is now running sensor sensor"));
  else
    Serial.println(F("Something went wrong during sensor init! sensor"));
  Serial.println();
}

/* 
  *   brief select to the rgba sensor using the SparkFun_APDS9960
  *  
  *   param 
  *   
  *   return true if the selection to the sensor was succesfull
  *
  */

bool RGBASensor::i2cMuxSelectMe()
{
  if (addr_ > 7)
    return false;
  Wire.beginTransmission(MUX_ADDR);
  Wire.write(1 << addr_);
  Wire.endTransmission();
  return true;
}

RGBAResult RGBASensor::read()
{
  RGBAResult result;
  result.IsReadOk = true;
  i2cMuxSelectMe();
  if (!apds_.readAmbientLight(result.Ambient))
  {
    result.IsReadOk = false;
    result.ErrCode = 0;
    Serial.print("error code 0");

  }
  if(!apds_.readRedLight(result.Red))
  {
    result.IsReadOk = false;
    result.ErrCode = 1;
    Serial.print("error code 1");
  }
  if(!apds_.readGreenLight(result.Green))
  {
    result.IsReadOk = false;
    result.ErrCode = 2;
    Serial.print("error code 2");
  }
  if(!apds_.readBlueLight(result.Blue))
  {
    result.IsReadOk = false;
    result.ErrCode = 3;
    Serial.print("error code 3");
  }
  if(!apds_.readProximity(result.Proximity))
  {
    result.IsReadOk = false;
    result.ErrCode = 4;
    Serial.print("error code 4");
  }
  else
  { //convert proximity to distance (cm)
    if (result.Proximity < 20) //min bound - read below it is not reliable
      result.Proximity = 20;
    result.Distance = 117.55 * pow(result.Proximity, -0.51); //result min val is 6, and max is 25 cm
  }
  return result;
}

/****************************void print()***************************
|@Goal: print rgba sensor values: ambient, red, green, blue, proximity
|@Comment: invoke read() before print() for updated values printing
********************************************************************/

void RGBASensor::print()
{
  RGBAResult read_res = read();
  if (read_res.IsReadOk)
  {
    Serial.println("------------RGBA Sensor Values------------");
    Serial.print("Ambient: "); Serial.print(read_res.Ambient);
    Serial.print(" | Red: ");   Serial.print(read_res.Red);
    Serial.print(" | Green: ");   Serial.print(read_res.Green);
    Serial.print(" | Blue: ");  Serial.print(read_res.Blue);
    Serial.print(" | Proximity: ");  Serial.println(read_res.Proximity);
  }
  else
  {
    Serial.println("[RGBSensor]: Error reading from sensor");
    
  }
}
