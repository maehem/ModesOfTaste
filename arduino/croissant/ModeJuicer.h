#ifndef ModeJuicer_h
#define ModeJuicer_h

// Servo
// Speaker
// PiComm ?

#include "Arduino.h"
#include "Noodle_DRV8838.h"


class ModeJuicer
{
  public:
    ModeJuicer(int enPin, int dirPin, int pwmPin, int saPin, int sbPin, int buttonPin);
    int begin();
    int doState();  // Return > 0 if faulted.
    boolean isFaulted();
    void tick();  // Motor sensor turned.
    
  private:
    void ModeJuicer::setForwardState();
    Noodle_DRV8838 _mot;
    int state;
    long watchdog;
    int movTicks;
    int rewindTicks;
    int _buttonPin;
};

#endif
