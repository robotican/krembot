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

/* Author: Elchay Rauper and Yair Shlomi*/

#ifndef MS_BUMPERS_H
#define MS_BUMPERS_H

#define BUMPERS_LEG A0


/** 
  *   @brief 
  *  
  *   @param 
  *   
  *   @return 
  *
  */


struct BumpersRes
{
  bool front = false,
       right = false,
       rear = false,
       left = false;
};


class MSBumpers
{

private:

public:

/* 
  *   brief Constructor 
  *  
  *   param 
  *   
  *   return 
  *
  */

  MSBumpers()
  {
    pinMode(BUMPERS_LEG, INPUT);
  }



/** 
  *   @brief Read if bumpers were pressed
  *  
  *   @param 
  *   
  *   @return the bumpers that was pressed
  *
  */

  BumpersRes read()
  {
    BumpersRes res;
    
    return res;
  }

/** 
  *   @brief prints which bumpers were pressed
  *  
  *   @param 
  *   
  *   @return 
  *
  */

  void print()
  {
    Serial.print("Bumpers Pressed: ");
    BumpersRes res;
    res = read();
    if (res.front)
      Serial.print("|FRONT|");
    if (res.rear)
      Serial.print("|REAR|");
    if (res.right)
      Serial.print("|RIGHT|");
    if (res.left)
      Serial.print("|LEFT|");
    Serial.println();
  }

  
};



#endif
