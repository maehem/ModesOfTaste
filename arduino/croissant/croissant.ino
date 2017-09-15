/** 
 *  Noodle's Croissant PCB code
 *  
 *  Pins:
 *    Mode:  A3:A2  (digital, use internal pullup)
 *    
 *     LED:
 *          R:  D10
 *          G:  D9
 *          B:  D11
 *          
 *     I2C:
 *        SDA:  A4
 *        SCL:  A5
 *        
 *     BUTTON:  D12  (use internal pullup)
 *   
 *    Motor:
 *        pwm:  D6
 *         S1:  D7
 *         S2:  D8
 *         EN:  A1
 *       !DIR:  A0
 *       
 *    Servo:
 *          A:  D6  (shared with motor)
 *          B:  D5
 *          
 *  SOLENNOID:  D2
 *  
 *  COLOR_SENSOR:  Uses I2C (except illum. LED)
 *       LED:   D4
 *       
 *      BEEP:   D3
 */
#include "ColorSensor.h"
#include "pitches.h"  // tones for the beeper

#include "ModeBean.h"
#include "ModeKlepto.h"
#include "ModeJuicer.h"
#include "ModeLitmus.h"

#include <avr/interrupt.h>

#define MODE_KLEPTO 0
#define MODE_LITMUS 1
#define MODE_JUICER 2
#define MODE_BEAN   3

#define MODE_PIN_0 A2
#define MODE_PIN_1 A3

#define LED_PIN_R  10
#define LED_PIN_G   9
#define LED_PIN_B  11

#define BUTTON_PIN 12

#define BEEP_PIN    3

#define SERVO_PIN_A 6
#define SERVO_PIN_B 5

// DON'T USE!
#define CS_LED_PIN  4

#define MOT_S1_PIN  7
#define MOT_S2_PIN  8
#define MOT_NDIR_PIN A0
#define MOT_EN_PIN  A1
#define MOT_PWM_PIN 6

#define SOLENOID_PIN 2



int mode = 4;  // Overwritten once we read the mode pins.
int state = 0;
//ColorSensor cs = ColorSensor(CS_LED);

ModeBean mBean(SERVO_PIN_A, BUTTON_PIN);
ModeJuicer mJuicer(MOT_EN_PIN, MOT_NDIR_PIN, MOT_PWM_PIN, MOT_S1_PIN, MOT_S2_PIN, BUTTON_PIN);
ModeKlepto mKlepto(MOT_EN_PIN, MOT_NDIR_PIN, MOT_PWM_PIN, MOT_S1_PIN, MOT_S2_PIN, SOLENOID_PIN);
ModeLitmus mLitmus(MOT_EN_PIN, MOT_NDIR_PIN, MOT_PWM_PIN, MOT_S1_PIN, MOT_S2_PIN, SOLENOID_PIN);

void setup() {
  // Read the mode bits.
  pinMode(MODE_PIN_0, INPUT_PULLUP);
  pinMode(MODE_PIN_1, INPUT_PULLUP);
  mode = (digitalRead(MODE_PIN_1)&0x1)<<1 | (digitalRead(MODE_PIN_0)&0x1);  // values 0 - 3
  // We could unset the mode pins here and use them for something else.
  Serial.begin(9600);



  // Maybe read this with an interrupt?
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED_PIN_R, OUTPUT);
  pinMode(LED_PIN_G, OUTPUT);
  pinMode(LED_PIN_B, OUTPUT);

  digitalWrite( LED_PIN_R, 1);
  digitalWrite( LED_PIN_G, 1);
  digitalWrite( LED_PIN_B, 1);

  switch (mode) {
    case MODE_KLEPTO:
      Serial.println("mode Klepto");

      tone(BEEP_PIN, NOTE_GS1, 1000);
      
      // Motor sensor interrupt setup
      cli();
      PCICR  |= 0b00000100; // Enables Port D Pin Change Interrupts
      PCMSK2 |= 0b10000000; // PCINT23  (Pin D7)
      sei();
      mKlepto.begin();    

      break;
    case MODE_LITMUS:
      Serial.println("mode Litmus");
      tone(BEEP_PIN, NOTE_GS2, 1000);
      
      // Motor sensor interrupt setup
      cli();
      PCICR  |= 0b00000100; // Enables Port D Pin Change Interrupts
      PCMSK2 |= 0b10000000; // PCINT23  (Pin D7)
      sei();
      
      mLitmus.begin();     
      break;
    case MODE_JUICER:
      Serial.println("mode Juicer");
      tone(BEEP_PIN, NOTE_GS2, 1000);
      
      // Motor sensor interrupt setup      
      cli();
      PCICR  |= 0b00000100; // Enables Port D Pin Change Interrupts
      PCMSK2 |= 0b10000000; // PCINT23  (Pin D7)
      sei();
      
      mJuicer.begin();
      tone(BEEP_PIN, NOTE_B2, 1000);
      break;
    case MODE_BEAN:
       Serial.println("mode Bean");
       mBean.begin();
       tone(BEEP_PIN, NOTE_DS2, 1000);
     break;
  } 
}

void loop() {
  switch (mode) {
    case MODE_KLEPTO:
      state = mKlepto.doState();
      break;
    case MODE_LITMUS:
      state = mLitmus.doState();
      break;
    case MODE_JUICER:
      state = mJuicer.doState();
      break;
    case MODE_BEAN:
      state = mBean.doState();
      break;
  }
  
  showState();  // Update the LED to reflect status.
}

void setLED( int r, int g, int b ) {
  analogWrite( LED_PIN_R, 255-r );
  analogWrite( LED_PIN_G, 255-g );
  analogWrite( LED_PIN_B, 255-b );
}

void showState() {
  switch (state) {
    case 0:
      analogWrite( LED_PIN_R, 255 );
      analogWrite( LED_PIN_G, 0 );
      analogWrite( LED_PIN_B, 255 );
      break;
    case 1:
      analogWrite( LED_PIN_R, 0 );
      analogWrite( LED_PIN_G, 0 );
      analogWrite( LED_PIN_B, 255 );
      break;
    case 2:
      analogWrite( LED_PIN_R, 255 );
      analogWrite( LED_PIN_G, 255 );
      analogWrite( LED_PIN_B, 0 );
      break;
    case 3:
      analogWrite( LED_PIN_R, 0 );
      analogWrite( LED_PIN_G, 255 );
      analogWrite( LED_PIN_B, 0 );
      break;
    case 4:
      analogWrite( LED_PIN_R, 128 );
      analogWrite( LED_PIN_G, 255 );
      analogWrite( LED_PIN_B, 128 );
      break;
    default:
      analogWrite( LED_PIN_R, 0 );
      analogWrite( LED_PIN_G, 255 );
      analogWrite( LED_PIN_B, 255 );
      break;      
  }
}

ISR(PCINT2_vect)
{
  mLitmus.tick();
  mKlepto.tick();
  mJuicer.tick();
}





