#ifndef WKC_KREMBO2PC_H
#define WKC_KREMBO2PC_H

#include "application.h"
#include "bit_converter.h"
#include "dac_bumpers.h"
#include "rgba_sensor.h"


//package from krembo to pc
//WKC - wireless krembo communication protocol

/**************************************************************************
 * |----------------------------------------- N BYTES ARRAY ----------------------------------------------|
 * |       0 (8 BITS FLAGS)        |   1    |   2   | 3-6 |7-10 |11-14|15-18|19-22|23-26|27-30|31-34|35-59|
 * |   |   |FB |REB|LB |RIB|BC |BF |   BL   |  BCL% |RGBF |RGBRE|RGBRI|RGBL |RGBFR|RGBFL|RGBRR|RGBRL| ID  |
 * |0-1|0-1|0-1|0-1|0-1|0-1|0-1|0-1| 0-100  | 0-100 |0-255|0-255|0-255|0-255|0-255|0-255|0-255|0-255|0-255|
 * |------------------------------------------------------------------------------------------------------|

 ************FLAGS**********
 * BC = Battery Charging = flage indicates whether battery is being charged
 * BF = Battery Full = flage indicates whether battery is Full

 ************BUMPERS FLAGS**********
 * Each bumper flag indicates whether bumper was pressed
 * FB = Front Bumper
 * RIB = Right Bumper
 * REB = Rear Bumper
 * RB = Right Bumper
 * LB = Left Bumper

 ************RGBA SENSORS**********
 * Each Rgba sensor is represented with 4 bytes (1 for proximity and 3 for rgb)
 * RGBF - front rgba
 * RGBRE - rear rgba
 * RGBRI - right rgba
 * RGBL - left rgba
 * RGBFR - front right rgba
 * RGBFL - front left rgba
 * RGBRR - rear right rgba
 * RGBRL - rear left rgba

 ************BATTERY DATA**********
 * BL = Battery Level %
 * BCL = Battery Charging Level % = current level from charger

 * ID = 24 bytes of Photon string id in bytes
 **************************************************************************/

//flags indexes
#define FLAGS_INDX 0
  #define BAT_FULL_BIT 0
  #define BAT_CHARGING_BIT 1
  #define BUMP_RIGHT_BIT 2
  #define BUMP_LEFT_BIT 3
  #define BUMP_REAR_BIT 4
  #define BUMP_FRONT_BIT 5

//battery indexes
#define BAT_LVL_INDX 1
#define BAT_CHRG_LVL_INDX 2

//rgba offset indexes
#define RGBA_PROX_OFFSET 0
#define RGBA_RED_OFFSET 1
#define RGBA_GREEN_OFFSET 2
#define RGBA_BLUE_OFFSET 3

//rgba start indexes
#define RGBA_FRONT_START_INDX 3
#define RGBA_REAR_START_INDX 7
#define RGBA_RIGHT_START_INDX 11
#define RGBA_LEFT_START_INDX 15
#define RGBA_FRONT_RIGHT_START_INDX 19
#define RGBA_FRONT_LEFT_START_INDX 23
#define RGBA_REAR_RIGHT_START_INDX 27
#define RGBA_REAR_LEFT_START_INDX 31

//id indexes
#define ID_START_INDX 35
#define ID_SIZE 24 //size of photon hardware id

#define MSG_SIZE ID_START_INDX + ID_SIZE //size bytes arr

#define RGBA16TO8(val) (uint8_t)(val / 300.0 * 255.0) //TODO: DELETE THIS//////////////////////////

class WKCKrembot2PC
{
private:
  String id_;
public:
  WKCKrembot2PC();
  uint8_t bat_lvl,
          bat_chrg_lvl;

  bool is_bat_chrgng = false,
       is_bat_full = false;

  RGBAResult rgba_front;
  RGBAResult rgba_rear;
  RGBAResult rgba_right;
  RGBAResult rgba_left;
  RGBAResult rgba_front_right;
  RGBAResult rgba_front_left;
  RGBAResult rgba_rear_right;
  RGBAResult rgba_rear_left;

  BumpersRes bumps;

  void toBytes(byte bytes_arr[]);
  uint16_t size();
  uint8_t map16RGBTo8RGB(uint16_t val)
  {
    return (uint8_t)(val / 300.0 * 255.0);
  }
};

#endif
