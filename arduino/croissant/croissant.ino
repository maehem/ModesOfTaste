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

#define CS_LED_PIN  4

#define MOT_S1_PIN  7
#define MOT_S2_PIN  8
#define MOT_NDIR_PIN A0
#define MOT_EN_PIN  A1
#define MOT_PWM_PIN 6

#define SOLENOID_PIN 2



int mode = 4;  // Overwritten once we read the mode pins.
//ColorSensor cs = ColorSensor(CS_LED);

ModeBean mBean(SERVO_PIN_A);
ModeJuicer mJuicer;
ModeKlepto mKlepto;
ModeLitmus mLitmus(CS_LED_PIN);

void setup() {
  // Read the mode bits.
  pinMode(MODE_PIN_0, INPUT_PULLUP);
  pinMode(MODE_PIN_1, INPUT_PULLUP);
  mode = (digitalRead(MODE_PIN_1)&0x1)<<1 | (digitalRead(MODE_PIN_0)&0x1);  // values 0 - 3
  // We could unset the mode pins here and use them for something else.

// Maybe read this with an interrupt?
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  switch (mode) {
    case MODE_KLEPTO:
      tone(BEEP_PIN, NOTE_GS1, 200);
      break;
    case MODE_LITMUS:
      tone(BEEP_PIN, NOTE_GS2, 200);
      //cs.begin();    
      break;
    case MODE_JUICER:
      tone(BEEP_PIN, NOTE_B2, 200);
      break;
    case MODE_BEAN:
       tone(BEEP_PIN, NOTE_DS2, 200);
     break;
  }
  //delay(200);
  //noTone(BEEP_PIN);
 
}

void loop() {
  switch (mode) {
    case MODE_KLEPTO:
      break;
    case MODE_LITMUS:
      break;
    case MODE_JUICER:
      break;
    case MODE_BEAN:
      break;
  }
}

void inspect() {
  //cs.led(TRUE);
  //cs.look();
  //cs.led(FALSE);
  //int red = cs.getRed();
  //int green = cs.getGreen();
  //int blue = cs.getBlue();
  
}

