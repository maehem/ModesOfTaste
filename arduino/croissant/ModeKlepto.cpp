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


#define STATE_STOPPED  0
#define STATE_FORWARD  1
#define STATE_REVERSE  2
#define STATE_TASTING  3
#define STATE_FAULTED  4

#define STATE_DELAY    4*60*1000UL
#define STATE_INITIAL_DELAY  1*60*1000UL

ModeKlepto::ModeKlepto(int enPin, int dirPin, int pwmPin, int saPin, int sbPin, int ledPin) {
  _mot = Noodle_DRV8838( enPin, dirPin, pwmPin, saPin, sbPin );
  _ledPin = ledPin;
}

int ModeKlepto::begin() {
  _mot.begin();
  watchdog = 0;
  state = STATE_STOPPED;
  firstPass = true;
  delay(500);
}

void ModeKlepto::tick() {
  _mot.tick();
}

int ModeKlepto::getTicks() {
  return _mot.getTicks();
}

boolean ModeKlepto::doState() {
  //  Move tape.
  switch ( state ) {
    case STATE_STOPPED:
      if (watchdog <= 0 ) {
        Serial.println("log state: forward");
        state = STATE_FORWARD;
        _mot.forward(150);  // Speed is 0-255. Tick's start counting at 0.
        watchdog = 30000;  // Watchdog
        movTicks = 1000;
      }
      break;
    case STATE_FORWARD:
      watchdog--;
      //Serial.println( _mot.getTicks() );
      if ( _mot.getTicks() > movTicks ) {
        // That's far enough.
        _mot.halt();
        delay(100);
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

        state = STATE_TASTING;  // Debug for no motor
      }
      break;
    case STATE_REVERSE:
      watchdog--;
      if ( _mot.getTicks() > rewindTicks ) {
        // We have rewound
        _mot.halt();
        Serial.println("log state: stopped");
        state = STATE_STOPPED;

        rewindTicks = 0;  // Clear it for next time.

        firstPass ? delay( STATE_INITIAL_DELAY) : delay(STATE_DELAY);
        firstPass = false;

      } else if ( watchdog <= 0 ) {
        // Watchdog!!!!  Motor not turning.
        _mot.halt();
        delay(100);
        Serial.println("log state: faulted");
        state = STATE_FAULTED;

        //state = STATE_STOPPED;  // Debug for no motor

      } else {
        // Animate the RGB
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

      Serial.println("log state: rewind");
      state = STATE_REVERSE;
      _mot.backward(150);
      watchdog = 10000; // Watchdog.

      break;
    case STATE_FAULTED:
    default:
      // How did we get here?
      // Flash RED LED.
      delay(100);
      break;
  }
  //  After 5 times, rewind and do again.
  return isFaulted();
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

