#ifndef BIT_CONVERTER_H
#define BIT_CONVERTER_H

#include "application.h"

struct BitConverter
{

  static void fromInt8(int8_t value, byte bytes[], int16_t start_index)
  {
   //TODO: implement
  }

  static uint8_t toInt8(byte bytes[], int16_t start_index)
  {
    return (uint8_t)bytes[start_index];
  }

  static void fromInt16(int16_t value, byte bytes[], int16_t start_index)
  {
  	bytes[start_index] = value & 0xFF;
  	bytes[start_index + 1] = (value >> 8) & 0xFF;
  }

  static uint16_t toInt16(byte bytes[], int16_t start_index)
  {
  	return ((bytes[start_index + 1] << 8) | bytes[start_index]);
  }

  //bit indexes are from right to left, e.g. [7,6,5,4,3,2,1,0]
  static void setBitInByte(byte & data_byte, byte bit_val, uint8_t bit_indx)
  {
    data_byte ^= (-bit_val ^ data_byte) & (1 << bit_indx);
  }

  static byte getBitInByte(byte data_byte, uint8_t bit_indx)
  {
    return (data_byte >> bit_indx) & 1;
  }
};

#endif
