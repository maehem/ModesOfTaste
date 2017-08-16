/*
 * Juice :
 * -turn motor X amount CW
 * -Pause
 * -turn motor X amount CCW
 * -pause for four minutes
 * 
 */
 
 #include "ModeJuicer.h"

#define STATE_STOPPED  0
#define STATE_FORWARD  1
#define STATE_REVERSE  2
#define STATE_FAULTED  3

#define STATE_DELAY    4*60*1000UL
#define STATE_INITIAL_DELAY  3*60*1000UL

ModeJuicer::ModeJuicer(int servoPin) {
  _servoPin = servoPin;
  state = STATE_STOPPED;
  firstPass = true;
  
}

int ModeJuicer::begin() {
  servo.attach(_servoPin);
  state = STATE_STOPPED;
  firstPass = true;
  delay(500);
}

boolean ModeJuicer::doState() {
  switch ( state ) {
    case STATE_STOPPED:
      delay(3000);
      state = STATE_FORWARD;
      break;
    case STATE_FORWARD:
      Serial.println("log state: forward");
      servo.write(90);
      delay(5000);
      state = STATE_REVERSE;
      break;
    case STATE_REVERSE:
      servo.write(0);
      firstPass ? delay( STATE_INITIAL_DELAY) : delay(STATE_DELAY);
      firstPass = false;
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
  return isFaulted();
}
