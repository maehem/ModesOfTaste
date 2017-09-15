#ifndef ModeBean_h
#define ModeBean_h
#include "Arduino.h"

// PiComm
#include <Servo.h>

class ModeBean
{
  public:
    ModeBean(int servoPin, int buttonPin);
    int begin();
    int doState();  // Return > 0 if faulted.
    boolean isFaulted();
    
  private:
    int _servoPin;
    int _buttonPin;
    int state;
    Servo servo;
};

#endif

