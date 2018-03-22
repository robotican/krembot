// Created by Elchay Rauper
// Last modified on 12/11/2015

// This library allow setting BlueSkyTimer

#ifndef BlueSkyTimer_H
#define BlueSkyTimer_H
#include "application.h"

class BlueSkyTimer
{
private:
    unsigned long start_time_;
    unsigned long end_time_;
    unsigned long period_;
    bool started_;

public:
    BlueSkyTimer();
    void setPeriod(unsigned long period);
    void start(unsigned long period);
    void startOver();
    bool finished();
    void reset();
    void delay(unsigned int seconds);
};

#endif
