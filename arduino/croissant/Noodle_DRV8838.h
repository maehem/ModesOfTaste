#ifndef Noodle_DRV8838_h
#define Noodle_DRV8838_h

// Motor
// Speaker
// Solennoid B
// PiComm ?

#define MODE_STP 0
#define MODE_FWD 1
#define MODE_BAK 2

class Noodle_DRV8838
{
  public:
    Noodle_DRV8838(); // Dummy object.
    Noodle_DRV8838( int enPin, int dirPin, int pwmPin, int saPin, int sbPin);  // Motor pins
    int begin();
    void forward(int spd);  // 0-255
    void backward(int spd); // 0-255
    void halt();
    void tick(); // The motor shaft is turning
    int getTicks();
   
    
  private:
    int _enPin, _dirPin, _pwmPin, _saPin, _sbPin;
    int _tickCount;
    int _mode;
};

#endif
