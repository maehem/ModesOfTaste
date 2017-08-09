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
    boolean doState();  // Return > 0 if faulted.
    boolean isFaulted();
   
  private:
    int _solPin;
    ColorSensor _cs;
    Noodle_DRV8838 _mot;
    int stateCount;
    int movTicks;
    int state;
    int rewindTicks;
    int tasteCount;
};

#endif

