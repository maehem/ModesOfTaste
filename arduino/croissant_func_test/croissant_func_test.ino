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
#include "Adafruit_TCS34725.h"

#define MODE_PIN_0 A2
#define MODE_PIN_1 A3

#define LED_PIN_R  10
#define LED_PIN_G   9
#define LED_PIN_B  11
#define LED_PIN_13  13

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
int hasColorSensor = 0;
Adafruit_TCS34725 tcs;

 void setup() {
  Serial.begin(9600);

  Serial.println("NoodleFeet Croissant - Board Tester");
  // Read the mode bits.
  pinMode(MODE_PIN_0, INPUT_PULLUP);
  pinMode(MODE_PIN_1, INPUT_PULLUP);
  mode = (digitalRead(MODE_PIN_1)&0x1)<<1 | (digitalRead(MODE_PIN_0)&0x1);  // values 0 - 3
  // We could unset the mode pins here and use them for something else.
  Serial.print("Mode:  ");
  Serial.println( mode );

  // Maybe read this with an interrupt?
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(LED_PIN_13, OUTPUT);

  // LEDs
  pinMode(LED_PIN_R, OUTPUT);
  pinMode(LED_PIN_G, OUTPUT);
  pinMode(LED_PIN_B, OUTPUT);
  digitalWrite(LED_PIN_R,1);
  digitalWrite(LED_PIN_G,1);
  digitalWrite(LED_PIN_B,1);

  pinMode(SOLENOID_PIN, OUTPUT);

  pinMode(MOT_PWM_PIN,  OUTPUT);
  pinMode(MOT_EN_PIN,   OUTPUT);
  pinMode(MOT_NDIR_PIN, OUTPUT);
  pinMode(MOT_S1_PIN,    INPUT);  // PULLUP?
  pinMode(MOT_S2_PIN,    INPUT);

  // Hook CS_LED pin on board (pin 7) to the pin next to it (pin 6)
  // to control the LED over I2C using the AdaFruit Library.
  tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
  // Turn CS LED on and off four times.
  if (tcs.begin()) {
    Serial.println("Found color sensor");
    hasColorSensor = 1;
    tcs.setInterrupt(true);      // turn off LED
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    hasColorSensor = 0;
  }
  delay(200);
}

void loop() {
  // Test LED.  Cycle RGB
  Serial.print("RGB LED Test ===>  ");
  Serial.print("Red   ");
  digitalWrite( LED_PIN_R, 0 );
  delay(500);
  digitalWrite( LED_PIN_R, 1 );
  delay(500);
  Serial.print("Green   ");
  digitalWrite( LED_PIN_G, 0 );
  delay(500);
  digitalWrite( LED_PIN_G, 1 );
  delay(500);
  Serial.print("Blue");
  digitalWrite( LED_PIN_B, 0 );
  delay(500);
  digitalWrite( LED_PIN_B, 1 );
  delay(500);
  Serial.println();
  
  int i=0;
  // Test mode count. Flash D13 n+1 times
  Serial.print( "Flash D13 LED [" );
  Serial.print( mode+1 );
  Serial.println( "] times.");
  for ( int r=0; r<5; r++) {
    for ( i=0; i< (mode+1); i++ ) {
      digitalWrite( LED_PIN_13, 1 );
      delay(100);
      digitalWrite( LED_PIN_13, 0 );
      delay(300);
    }
    delay(600);
  }
  
  // Test beeper
  Serial.println( "BEEEEEEEEEEEEEP!!!!" );
  for ( i=100; i< 8000; i*=2 ) {
    tone( BEEP_PIN, i, 200 );
    delay(100);
  }
  noTone(BEEP_PIN);
  
  // Test motor
  Serial.println( "Motor Test:" );
  int val = 0;
  digitalWrite( MOT_EN_PIN, 1);
  for ( int d=0; d<2; d++ ) {
    Serial.print( "   Dir: ");
    Serial.println( d );
    digitalWrite( MOT_NDIR_PIN, d );
    for ( i=0; i<256; i++ ) {
      analogWrite( MOT_PWM_PIN, i );
      delay(8);
    }
    for ( i=255; i>0; i-- ) {
      analogWrite( MOT_PWM_PIN, i );
      delay(8);
    }
  }
  analogWrite( MOT_PWM_PIN, 0); 
  digitalWrite(MOT_EN_PIN, 0);
  digitalWrite(MOT_NDIR_PIN, 0);
  
  
  // Test solenoid
  Serial.println("Test Solenoid");
  for ( i=0; i<5; i++) {
    digitalWrite( SOLENOID_PIN, 1);
    delay(500);
    digitalWrite( SOLENOID_PIN, 0);
    delay(500);
  }
  
  // Test button
  // Make this on an interupt?
  
  // Test color sensor
  if ( hasColorSensor ) {
    Serial.println( "Test Color Sensor::  ");
    uint16_t clr, red, green, blue;

    for (int i=0; i<10; i++) {
      tcs.setInterrupt(false);      // turn on LED
    
      delay(60);  // takes 50ms to read 
      
      tcs.getRawData(&red, &green, &blue, &clr);
    
      tcs.setInterrupt(true);  // turn off LED
    
  //    _red = red;
  //    _green = green;
  //    _blue = blue;
  //    _clr = clr;
      
      Serial.print("\tC:\t"); Serial.print(clr);
      Serial.print("\tR:\t"); Serial.print(red);
      Serial.print("\tG:\t"); Serial.print(green);
      Serial.print("\tB:\t"); Serial.print(blue);
      Serial.println();
    
  //    // Figure out some basic hex code for visualization
  //    uint32_t sum = clr;
  //    float r, g, b;
  //    r = red; r /= sum;
  //    g = green; g /= sum;
  //    b = blue; b /= sum;
  //    r *= 256; g *= 256; b *= 256;
  //    Serial.print("\t");
  //    Serial.print((int)r, HEX); Serial.print((int)g, HEX); Serial.print((int)b, HEX);
  //    Serial.println(); 

      delay(500);
    }
  } else {
    Serial.println( "Test Color Sensor  ==>   Not installed!");
  }


  Serial.println( "End Tests" );

}
