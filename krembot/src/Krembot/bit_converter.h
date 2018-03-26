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

/* Author: Elchay Rauper */

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
