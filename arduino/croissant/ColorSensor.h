#ifndef ColorSensor_h
#define ColorSensor_h

#include "Wire.h"
#include "Adafruit_TCS34725.h"

class ColorSensor
{
  public:
    ColorSensor();
    int begin();
    void led( boolean on );
    void look();
    int getRed();
    int getGreen();
    int getBlue();
    int getClear();
    
  private:
    int _clr, _red, _green, _blue;
    Adafruit_TCS34725 tcs;
};

#endif

