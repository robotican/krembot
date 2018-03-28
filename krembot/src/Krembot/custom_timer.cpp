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


#include "custom_timer.h"

CustomTimer::CustomTimer() 
{
    started_ = false;
    period_ = 1000;
}

void CustomTimer::setPeriod(unsigned long period)
{
  period_ = period;
}
void CustomTimer::start(unsigned long period)
{
    if (!started_)
    {
        period_ = period;
        start_time_ = millis();
        started_ = true;
    }
}

void CustomTimer::start()
{
    if (!started_)
    {
        start_time_ = millis();
        started_ = true;
    }
}
void CustomTimer::startOver() //override original start time, and start again
{
    start_time_ = millis();
    if (!started_)
        started_ = true;
}
//return true if CustomTimer has finished
bool CustomTimer::finished()
{
    if (started_)
    {
        end_time_ = millis();
        if (end_time_ - start_time_ >= period_)
        {
            started_ = false;
        }
    }
    return !started_;
}

void CustomTimer::reset() {started_ = false;}

void CustomTimer::delay(unsigned int period)
{
  unsigned long start = millis();
  while (millis() - start < period) {};
}
