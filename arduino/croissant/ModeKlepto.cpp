/*
    Kepto : (for pi, tweet about value logged every hour)
     -turn motor x amount CW.
     -pause
     -turn on LED
     -take picture with pi camera and store in folder
     -Pause
     -turn motor x amount CCW
     -pause for four minutes
*/

#include "ModeKlepto.h"

#define STATE_INIT     0
#define STATE_STOPPED  1
#define STATE_FORWARD  2
#define STATE_REVERSE  3
#define STATE_TASTING  4
#define STATE_FAULTED  5

// 2 minutes after a forward and 2 minutes after reverse
#define STATE_DELAY    2*30*1000UL
#define STATE_INITIAL_DELAY  1*30*1000UL

// debug values
//#define STATE_DELAY    4*1000UL
//#define STATE_INITIAL_DELAY  1*1000UL

#define FORWARD_COUNT 5

ModeKlepto::ModeKlepto(int enPin, int dirPin, int pwmPin, int saPin, int sbPin, int ledPin) {
  _mot = Noodle_DRV8838( enPin, dirPin, pwmPin, saPin, sbPin );
  _ledPin = ledPin;
}

int ModeKlepto::begin() {
  _mot.begin();
  watchdog = 0;
  state = STATE_INIT;
  delay(500);
}

void ModeKlepto::tick() {
  _mot.tick();
}

int ModeKlepto::getTicks() {
  return _mot.getTicks();
}

int ModeKlepto::doState() {
  switch ( state ) {
    case STATE_INIT:
      delay(STATE_INITIAL_DELAY);
      state = STATE_STOPPED;
      break;
    case STATE_STOPPED:
      if ( rewindTicks > 0 ) {
        Serial.println("log state: reverse");
        state = STATE_REVERSE;
        _mot.backward(150);
        watchdog = 30000;
        movTicks = rewindTicks;
      } else {
        Serial.println("log state: forward");
        state = STATE_FORWARD;
        _mot.forward(150);  // Speed is 0-255. Tick's start counting at 0.
        watchdog = 30000;  // Watchdog
        movTicks = 200;
      }
      break;
    case STATE_FORWARD:
      watchdog--;
      //Serial.println( _mot.getTicks() );
      if ( _mot.getTicks() > movTicks ) {
        // That's far enough.
        _mot.halt();
        //fwdCount++;
        delay(600);
        rewindTicks += _mot.getTicks();
        Serial.println("log state: tasting");
        state = STATE_TASTING;
        movTicks = 0;
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
      if ( _mot.getTicks() > movTicks ) {
        // That's far enough.
        _mot.halt();
        //fwdCount++;
        delay(600);
        rewindTicks = 0;
        Serial.println("log state: stop");
        state = STATE_STOPPED;
        movTicks = 0;
        delay(STATE_DELAY);
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
    case STATE_TASTING:
      digitalWrite( _ledPin, 1 );
      //  Snap a picture
      //delay(500);
      Serial.println( "snap" );
      delay(5000);

      digitalWrite( _ledPin, 0);

      // Check for response from Pi.  Set a flag if no response.

      //  Wait a few seconds.
      delay(2000);

      Serial.println("log state: stop");
      state = STATE_STOPPED;
      delay(STATE_DELAY);

      //_mot.backward(150);
      //watchdog = 10000; // Watchdog.

      break;
    case STATE_FAULTED:
    default:
      // How did we get here?
      // Flash RED LED.
      delay(100);
      break;
  }
  return state;
}

// Might not need these.
void ModeKlepto::forward(int spd) {
  _mot.forward(spd);
}

void ModeKlepto::backward(int spd)  {
  _mot.backward(spd);
}

void ModeKlepto::halt() {
  _mot.halt();
}

boolean ModeKlepto::isFaulted() {
  return state >= STATE_FAULTED;
}

