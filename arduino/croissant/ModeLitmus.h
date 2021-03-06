#ifndef ModeLitmus_h
#define ModeLitmus_h

// Solenoid
// Motor
// Color Sensor
// PiComm

#include "ColorSensor.h"
#include "Noodle_DRV8838.h"

class ModeLitmus
{
  public:
    ModeLitmus(int enPin, int dirPin, int pwmPin, int saPin, int sbPin, int solPin);
    int begin();
    void tick();  // Motor sensor turned.
    int getTicks();
    void forward(int spd);  // 0-255
    void backward(int spd); // 0-255
    void halt();
    int doState();  // Return > 0 if faulted.
    boolean isFaulted();
    int getRed();
    int getGreen();
    int getBlue();
   
  private:
    int _solPin;
    ColorSensor _cs;
    Noodle_DRV8838 _mot;
    long watchdog;
    int movTicks;
    int state;
};

#endif

