#ifndef ColorSensor_h
#define ColorSensor_h

#include "Adafruit_TCS34725.h"

class ColorSensor
{
  public:
    ColorSensor(int ledPin);
    int begin();
    void led( int on );
    void look();
    int getRed();
    int getGreen();
    int getBlue();
    int getClear();
    
  private:
    int _clr, _red, _green, _blue;
    int _pin;
    Adafruit_TCS34725 tcs;
};

#endif

