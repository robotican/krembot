// Created by Elchay Rauper
// Last modified on 12/11/2015

#include "bluesky_timer.h"

BlueSkyTimer::BlueSkyTimer() {started_ = false;}

void BlueSkyTimer::setPeriod(unsigned long period)
{
  period_ = period;
}
void BlueSkyTimer::start(unsigned long period)
{
    if (!started_)
    {
        period_ = period;
        start_time_ = millis();
        started_ = true;
    }
}
void BlueSkyTimer::startOver() //override original start time, and start again
{
    start_time_ = millis();
    if (!started_)
        started_ = true;
}
//return true if BlueSkyTimer has finished
bool BlueSkyTimer::finished()
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

void BlueSkyTimer::reset() {started_ = false;}

void BlueSkyTimer::delay(unsigned int period)
{
  unsigned long start = millis();
  while (millis() - start < period) {};
}
