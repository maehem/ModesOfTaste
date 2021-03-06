#ifndef ModeKlepto_h
#define ModeKlepto_h
#include "Arduino.h"

#include "Noodle_DRV8838.h"

// Motor
// PiComm

class ModeKlepto
{
  public:
    ModeKlepto(int enPin, int dirPin, int pwmPin, int saPin, int sbPin, int ledPin);
    int begin();
    void tick();  // Motor sensor turned.
    int getTicks();
    void forward(int spd);  // 0-255
    void backward(int spd); // 0-255
    void halt();
    int doState();  // Return > 0 if faulted.
    boolean isFaulted();
        
  private:
    int _ledPin;
    Noodle_DRV8838 _mot;
    int state;
    int watchdog;
    int movTicks;
    int rewindTicks;
    //int fwdCount;
};

#endif

