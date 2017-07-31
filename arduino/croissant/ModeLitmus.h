#ifndef ModeLitmus_h
#define ModeLitmus_h

// Solenoid
// Motor
// Color Sensor
// PiComm

#include "Adafruit_TCS34725.h"

class ModeLitmus
{
  public:
    ModeLitmus(int ledPin);
    int begin();
    
  private:
    int _pin;
    Adafruit_TCS34725 tcs;
};

#endif

