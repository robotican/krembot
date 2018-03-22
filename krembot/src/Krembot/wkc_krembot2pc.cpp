
#include "wkc_krembot2pc.h"

WKCKrembot2PC::WKCKrembot2PC()
{
  id_ = System.deviceID(); //get 24 long id string
}

void WKCKrembot2PC::toBytes(byte bytes_arr[])
{
  byte flags_byte = 0;

  //battery flags
  BitConverter::setBitInByte(flags_byte,
                             is_bat_full,
                             BAT_FULL_BIT);

  BitConverter::setBitInByte(flags_byte,
                            is_bat_chrgng,
                            BAT_CHARGING_BIT);

  //bumpers flags
  BitConverter::setBitInByte(flags_byte,
                             bumps.front,
                             BUMP_FRONT_BIT);

  BitConverter::setBitInByte(flags_byte,
                            bumps.rear,
                            BUMP_REAR_BIT);

  BitConverter::setBitInByte(flags_byte,
                             bumps.right,
                             BUMP_RIGHT_BIT);

  BitConverter::setBitInByte(flags_byte,
                            bumps.left,
                            BUMP_LEFT_BIT);

  bytes_arr[FLAGS_INDX] = flags_byte;
  bytes_arr[BAT_LVL_INDX] = bat_lvl;
  bytes_arr[BAT_CHRG_LVL_INDX] = bat_chrg_lvl;

  //rgba sensors
  bytes_arr[RGBA_FRONT_START_INDX + RGBA_PROX_OFFSET] = rgba_front.Distance;
  bytes_arr[RGBA_FRONT_START_INDX + RGBA_RED_OFFSET] = RGBA16TO8(rgba_front.Red);
  bytes_arr[RGBA_FRONT_START_INDX + RGBA_GREEN_OFFSET] = RGBA16TO8(rgba_front.Green);
  bytes_arr[RGBA_FRONT_START_INDX + RGBA_BLUE_OFFSET] = RGBA16TO8(rgba_front.Blue);

  bytes_arr[RGBA_REAR_START_INDX + RGBA_PROX_OFFSET] = rgba_rear.Distance;
  bytes_arr[RGBA_REAR_START_INDX + RGBA_RED_OFFSET] = RGBA16TO8(rgba_rear.Red);
  //Serial.println(rgba_rear.Red);///////////////////////////////////////////////////////////////////
  //Serial.print("--->"); Serial.println(RGBA16TO8(rgba_rear.Red));//////////////////////////////
  bytes_arr[RGBA_REAR_START_INDX + RGBA_GREEN_OFFSET] = RGBA16TO8(rgba_rear.Green);
  bytes_arr[RGBA_REAR_START_INDX + RGBA_BLUE_OFFSET] = RGBA16TO8(rgba_rear.Blue);

  bytes_arr[RGBA_RIGHT_START_INDX + RGBA_PROX_OFFSET] = rgba_right.Distance;
  bytes_arr[RGBA_RIGHT_START_INDX + RGBA_RED_OFFSET] = RGBA16TO8(rgba_right.Red);
  bytes_arr[RGBA_RIGHT_START_INDX + RGBA_GREEN_OFFSET] = RGBA16TO8(rgba_right.Green);
  bytes_arr[RGBA_RIGHT_START_INDX + RGBA_BLUE_OFFSET] = RGBA16TO8(rgba_right.Blue);

  bytes_arr[RGBA_LEFT_START_INDX + RGBA_PROX_OFFSET] = rgba_left.Distance;
  bytes_arr[RGBA_LEFT_START_INDX + RGBA_RED_OFFSET] = RGBA16TO8(rgba_left.Red);
  bytes_arr[RGBA_LEFT_START_INDX + RGBA_GREEN_OFFSET] = RGBA16TO8(rgba_left.Green);
  bytes_arr[RGBA_LEFT_START_INDX + RGBA_BLUE_OFFSET] = RGBA16TO8(rgba_left.Blue);

  bytes_arr[RGBA_FRONT_RIGHT_START_INDX + RGBA_PROX_OFFSET] = rgba_front_right.Distance;
  bytes_arr[RGBA_FRONT_RIGHT_START_INDX + RGBA_RED_OFFSET] = RGBA16TO8(rgba_front_right.Red);
  bytes_arr[RGBA_FRONT_RIGHT_START_INDX + RGBA_GREEN_OFFSET] = RGBA16TO8(rgba_front_right.Green);
  bytes_arr[RGBA_FRONT_RIGHT_START_INDX + RGBA_BLUE_OFFSET] = RGBA16TO8(rgba_front_right.Blue);

  bytes_arr[RGBA_FRONT_LEFT_START_INDX + RGBA_PROX_OFFSET] = rgba_front_left.Distance;
  bytes_arr[RGBA_FRONT_LEFT_START_INDX + RGBA_RED_OFFSET] = RGBA16TO8(rgba_front_left.Red);
  bytes_arr[RGBA_FRONT_LEFT_START_INDX + RGBA_GREEN_OFFSET] = RGBA16TO8(rgba_front_left.Green);
  bytes_arr[RGBA_FRONT_LEFT_START_INDX + RGBA_BLUE_OFFSET] = RGBA16TO8(rgba_front_left.Blue);

  bytes_arr[RGBA_REAR_RIGHT_START_INDX + RGBA_PROX_OFFSET] = rgba_rear_right.Distance;
  bytes_arr[RGBA_REAR_RIGHT_START_INDX + RGBA_RED_OFFSET] = RGBA16TO8(rgba_rear_right.Red);
  bytes_arr[RGBA_REAR_RIGHT_START_INDX + RGBA_GREEN_OFFSET] = RGBA16TO8(rgba_rear_right.Green);
  bytes_arr[RGBA_REAR_RIGHT_START_INDX + RGBA_BLUE_OFFSET] = RGBA16TO8(rgba_rear_right.Blue);

  bytes_arr[RGBA_REAR_LEFT_START_INDX + RGBA_PROX_OFFSET] = rgba_rear_left.Distance;
  bytes_arr[RGBA_REAR_LEFT_START_INDX + RGBA_RED_OFFSET] = RGBA16TO8(rgba_rear_left.Red);
  bytes_arr[RGBA_REAR_LEFT_START_INDX + RGBA_GREEN_OFFSET] = RGBA16TO8(rgba_rear_left.Green);
  bytes_arr[RGBA_REAR_LEFT_START_INDX + RGBA_BLUE_OFFSET] = RGBA16TO8(rgba_rear_left.Blue);


  byte id_bytes[ID_SIZE];
  //particle apigetbyte() function is buggy,
  //therefore adding one is needed to insure
  //last byte is returned to array
  id_.getBytes(id_bytes, ID_SIZE+1);
  for(int i=0; i < ID_SIZE; i++)
  {
    bytes_arr[i + ID_START_INDX] = id_bytes[i];
  }
}

uint16_t WKCKrembot2PC::size()
{
  return MSG_SIZE;
}
