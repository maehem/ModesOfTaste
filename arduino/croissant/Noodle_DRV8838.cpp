#include "Noodle_DRV8838.h"
#include "arduino.h"

Noodle_DRV8838::Noodle_DRV8838() {}

Noodle_DRV8838::Noodle_DRV8838(int enPin, int dirPin, int pwmPin, int saPin, int sbPin) {
  _enPin  = enPin;
  _dirPin = dirPin;
  _pwmPin = pwmPin;
  _saPin  = saPin;
  _sbPin  = sbPin;

  _mode = MODE_STP;
  _tickCount = 0;
  
}


int Noodle_DRV8838::begin() {
  pinMode( _enPin, OUTPUT );
  pinMode( _dirPin, OUTPUT );
  // pwm pin is analog and does not require pinMode()

  pinMode( _saPin, INPUT );
  pinMode( _sbPin, INPUT );
}

void Noodle_DRV8838::forward(int spd) {
  _mode = MODE_FWD;
  _tickCount = 0;
  digitalWrite( _enPin, true   );
  digitalWrite( _dirPin, false );
  analogWrite( _pwmPin, 170   );
  delay(30); // Motor needs time to windup.
  analogWrite( _pwmPin, spd   );
  //delay(40);

}

void Noodle_DRV8838::backward(int spd) { 
  _mode = MODE_BAK;
  _tickCount = 0;
  digitalWrite(  _enPin, true );
  digitalWrite( _dirPin, true );
  analogWrite( _pwmPin, 200   );
  delay(30); // Motor needs time to windup.
  analogWrite( _pwmPin, spd );
  //delay(40);
}

void Noodle_DRV8838::halt() {
  _mode = MODE_STP;
  digitalWrite(  _enPin, false );
  digitalWrite( _dirPin, false );
   analogWrite( _pwmPin, 0   );
}

void Noodle_DRV8838::tick() {
  if (_mode != MODE_STP) {
    _tickCount++;
  }
}

int Noodle_DRV8838::getTicks() {
    return _tickCount;
}

