#include "ModeLitmus.h"

#define STATE_STOPPED  0
#define STATE_FORWARD  1
#define STATE_REVERSE  2
#define STATE_TASTING  3
#define STATE_FAULTED  4


ModeLitmus::ModeLitmus(int enPin, int dirPin, int pwmPin, int saPin, int sbPin, int solPin) {
  _mot = Noodle_DRV8838( enPin, dirPin, pwmPin, saPin, sbPin );
  _cs = ColorSensor();
  _solPin = solPin;
}
    
int ModeLitmus::begin() {
  _cs.begin();
  _mot.begin();
  stateCount = 0;
  state = STATE_STOPPED;
  delay(500);
}

void ModeLitmus::tick() {
  _mot.tick();
}

int ModeLitmus::getTicks() {
  return _mot.getTicks();
}

boolean ModeLitmus::doState() {
      //  Move tape.
      stateCount--;
      switch( state ) {
        case STATE_STOPPED:
          if (stateCount <= 0 ) {
            Serial.println("log state: forward");
            state = STATE_FORWARD;
            _mot.forward(150);  // Speed is 0-255. Tick's start counting at 0.
            stateCount = 30000;  // Watchdog
            movTicks = 1000;
          }
          break;
        case STATE_FORWARD:
          //Serial.println( _mot.getTicks() );
          if ( _mot.getTicks() > movTicks ) {
            // That's far enough.
            _mot.halt();
            delay(100);
            rewindTicks += _mot.getTicks();
            Serial.println("log state: tasting");
            state = STATE_TASTING;
            stateCount = 10000;
            movTicks = 0;
          }
          else if ( stateCount <= 0 ) {
            // Watchdog!!!!  Motor not turning.
            _mot.halt();
            delay(100);
            Serial.println("log state: faulted");
            state = STATE_FAULTED;
          }
          break;
        case STATE_REVERSE:
          if ( _mot.getTicks() > rewindTicks ) {
             // We have rewound
            _mot.halt();
            Serial.println("log state: stopped");
            state = STATE_STOPPED;

            rewindTicks = 0;  // Clear it for next time.
            stateCount = 1000;  // Wait this long.
            delay(1000);
          }
          break;
        case STATE_TASTING:
            digitalWrite( _solPin, 1 );
            delay(600);
            digitalWrite( _solPin, 0 );
            delay(600);
            
            //  Read color.  Dwell.  Foward.  Four times.
            //  Look at the color.
            _cs.led(true);
            delay(60);
            _cs.look();
            _cs.led(false);
            //  Tell Pi the color.
            Serial.print( "log color " );
            Serial.print( " " );
            Serial.print( _cs.getRed() );
            Serial.print( " " );
            Serial.print( _cs.getBlue() );
            Serial.print( " " );
            Serial.print( _cs.getGreen() );
            Serial.print( " " );
            Serial.print( _cs.getClear() );
            Serial.println();  // Do we need   \n and \r  ?

            // Set the LED to the color.
            
            // Check for response from Pi.  Set a flag if no response.
            
            //  Wait a few seconds.
            delay(2000);

          tasteCount++;
          if ( tasteCount > 4 ) {
            Serial.println("log state: rewind");
            state = STATE_REVERSE;
            tasteCount = 0;
            _mot.backward(150);
            stateCount = 10000; // Watchdog.
          } else {
            // Forward
            Serial.println("log state: forward");
            state = STATE_FORWARD;
            movTicks = 1000;  // How many ticks to move.
            _mot.forward(150);
            stateCount = 10000;  // Watchdog
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
      return isFaulted();
}

// Might not need these.
void ModeLitmus::forward(int spd) {
  _mot.forward(spd);
}

void ModeLitmus::backward(int spd)  {
  _mot.backward(spd);
}

void ModeLitmus::halt() {
  _mot.halt();
}

boolean ModeLitmus::isFaulted() {
  return state>=STATE_FAULTED;
}

