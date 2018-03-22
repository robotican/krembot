#ifndef RGB_LED_H
#define RGB_LED_H

#include "application.h"

#define RED_LEG TX
#define GREEN_LEG RX
#define BLUE_LEG WKP

class RGBLed
{
private:

public:
  RGBLed();
  void write(uint8_t red_val,
             uint8_t green_val,
             uint8_t blue_val);

};

#endif //DEBUGER_H
