#ifndef ModeJuicer_h
#define ModeJuicer_h

// Servo
// Speaker
// PiComm ?

#include "Arduino.h"

// PiComm
#include <Servo.h>

class ModeJuicer
{
  public:
    ModeJuicer(int servoPin);
    int begin();
    boolean doState();  // Return > 0 if faulted.
    boolean isFaulted();
    
  private:
    int _servoPin;
    int state;
    Servo servo;
    boolean firstPass;
};

#endif
