#ifndef EEPROM_DATA
#define EEPROM_DATA

#include "application.h"

#define BASE_DATA_ADDR 0

struct BaseData
{
  int8_t right_offset = 0,
         left_offset = 0;
  uint8_t bias = 0;
}

#endif
