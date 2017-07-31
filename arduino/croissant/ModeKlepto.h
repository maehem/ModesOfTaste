#ifndef ModeKlepto_h
#define ModeKlepto_h

// Motor
// PiComm

class ModeKlepto
{
  public:
    ModeKlepto();
    int begin();
    
  private:
    int _pin;
};

#endif

