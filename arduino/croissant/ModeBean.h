#ifndef ModeBean_h
#define ModeBean_h

// Motor
// PiComm

class ModeBean
{
  public:
    ModeBean(int servoPin);
    int begin();
    
  private:
    int _servoPin;
};

#endif

