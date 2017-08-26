/*
   Juice :
   -turn motor X amount CW
   -Pause
   -turn motor X amount CCW
   -pause for four minutes

*/

#include "ModeJuicer.h"

#define STATE_INITIAL  0
#define STATE_STOPPED  1
#define STATE_FORWARD  2
#define STATE_REVERSE  3
#define STATE_FAULTED  5

#define MOT_WATCHDOG 30000
#define MOV_TICKS 1200
#define MOT_SPEED 150

#define STATE_DELAY    4*60*1000UL
#define STATE_INITIAL_DELAY  3*60*1000UL

// debug values
//#define STATE_DELAY    4*1000UL
//#define STATE_INITIAL_DELAY  1*1000UL


ModeJuicer::ModeJuicer(int enPin, int dirPin, int pwmPin, int saPin, int sbPin, int buttonPin) {
  _mot = Noodle_DRV8838( enPin, dirPin, pwmPin, saPin, sbPin );
  _buttonPin = buttonPin;
  state = STATE_STOPPED;
}

int ModeJuicer::begin() {
  _mot.begin();
  watchdog = 0;
  state = STATE_INITIAL;
  rewindTicks = 0;

  while( !digitalRead(_buttonPin) ) {
    _mot.backward(255);
  }
  _mot.halt();
  delay(500);
}

int ModeJuicer::doState() {
  switch ( state ) {
    case STATE_INITIAL:
      Serial.println("log state: initial delay");
      delay(STATE_INITIAL_DELAY);
      setForwardState();

      break;
    case STATE_STOPPED:
      Serial.println("log state: stopped");
      delay(STATE_DELAY);

      setForwardState();

      break;
    case STATE_FORWARD:
      watchdog--;
      //Serial.println( _mot.getTicks() );
      if ( _mot.getTicks() >= movTicks ) {
        // That's far enough.
        _mot.halt();

//        Serial.print( "ticks moved: ");
//        Serial.print( _mot.getTicks() );
//        Serial.print( "   intended ticks: " );
//        Serial.println( movTicks);

        delay(3000);

        rewindTicks = _mot.getTicks();
        Serial.println("log state: reverse");
        Serial.print("rewind ticks: ");
        Serial.println(rewindTicks);
        state = STATE_REVERSE;
        watchdog = MOT_WATCHDOG;  // Watchdog
        movTicks = rewindTicks;
        _mot.backward(MOT_SPEED);  // Speed is 0-255. Tick's start counting at 0.
      }
      else if ( watchdog <= 0 ) {
        // Watchdog!!!!  Motor not turning.
        _mot.halt();
        delay(100);
        Serial.println("log state: faulted");
        state = STATE_FAULTED;

        //state = STATE_TASTING;  // Debug for no motor
      }
      break;
    case STATE_REVERSE:
      watchdog--;
      //Serial.println( _mot.getTicks() );
      if ( _mot.getTicks() >= movTicks ) {
        // That's far enough.
        _mot.halt();
//        Serial.print( "ticks moved: ");
//        Serial.print( _mot.getTicks() );
//        Serial.print( "   intended ticks: " );
//        Serial.println( movTicks);

        delay(300);
        rewindTicks = 0;

        state = STATE_STOPPED;
      }
      else if ( watchdog <= 0 ) {
        if ( _mot.getTicks() == 0 ) {
          // Watchdog!!!!  Motor not turning.
          _mot.halt();
          delay(100);
          Serial.println("log state: faulted");
          Serial.print("mot ticks: ");
          Serial.println(_mot.getTicks());
          state = STATE_FAULTED;
        } else {
          watchdog = MOT_WATCHDOG;
        }
      }
      break;
    case STATE_FAULTED:
    default:
      // How did we get here?
      // Flash RED LED.
      delay(100);
      break;
  }
  //  After 5 times, rewind and do again.
  return state;
}

void ModeJuicer::setForwardState() {
  Serial.println("log state: forward");
  state = STATE_FORWARD;
  _mot.forward(MOT_SPEED);  // Speed is 0-255. Tick's start counting at 0.
  watchdog = MOT_WATCHDOG;  // Watchdog
  movTicks = MOV_TICKS;
}

void ModeJuicer::tick() {
  _mot.tick();
}

boolean ModeJuicer::isFaulted() {
  return state >= STATE_FAULTED;
}
