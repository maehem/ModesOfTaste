#ifndef ModeBean_h
#define ModeBean_h
#include "Arduino.h"

// PiComm
#include <Servo.h>

class ModeBean
{
  public:
    ModeBean(int servoPin);
    int begin();
    int doState();  // Return > 0 if faulted.
    boolean isFaulted();
    
  private:
    int _servoPin;
    int state;
    Servo servo;
    boolean firstPass;
};

#endif

