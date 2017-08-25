/*
 * Bean:
 * -turn servo 45 degrees CW
 * -pause
 * -turn servo 45 degrees CCW
 * -pause for four minutes
 */

#include "ModeBean.h"

#define STATE_INITIAL 0
#define STATE_STOPPED  1
#define STATE_FORWARD  2
#define STATE_REVERSE  3
#define STATE_FAULTED  5

//#define STATE_DELAY    4*60*1000UL
//#define STATE_INITIAL_DELAY  3*60*1000UL

// debug
#define STATE_DELAY    4*1000UL
#define STATE_INITIAL_DELAY  3*1000UL

ModeBean::ModeBean(int servoPin) {
  _servoPin = servoPin;
  state = STATE_INITIAL;  
}


int ModeBean::begin() {
  servo.attach(_servoPin);
  servo.write(90);

  delay(2000);
  servo.write(68);
}

int ModeBean::doState() {
  switch ( state ) {
    case STATE_INITIAL:
      delay(STATE_INITIAL_DELAY);
      break;
    case STATE_STOPPED:
      delay(1000);
      state = STATE_FORWARD;
      break;
    case STATE_FORWARD:
      Serial.println("log state: forward");
      servo.write(112);
      delay(2000);
      state = STATE_REVERSE;
      break;
    case STATE_REVERSE:
      servo.write(68);
      delay(STATE_DELAY);
      state = STATE_STOPPED;
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
