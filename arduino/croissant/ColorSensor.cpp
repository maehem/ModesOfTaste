/**
 *  Class wrapper for AdaFruit Color sensor library
 *  
 *  Adafruit example uses a gamma table to match measured color values
 *  to the RGB LED.   We might want to integrate that in the future.
 *  
 *  This file uses some code from the AdaFruit ColorView example.  No attribution
 *  information was included in the example, so no further attribution will be made here.
 *  
 */
 #include "ColorSensor.h"
 
ColorSensor::ColorSensor()
{
  _clr = 0;
  _red = 0;
  _green = 0;
  _blue = 0;
  tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
}

int ColorSensor::begin()
{
  Wire.begin();
  
  if (tcs.begin()) {
    Serial.println("Found color sensor");
    return 0;
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
    // return 1;
  }

  led(false);
}

void ColorSensor::led( boolean on)
{
  tcs.setInterrupt(!on);

//  if (on) {
//    digitalWrite(_pin, HIGH);
//  }
}

void ColorSensor::look()
{
  uint16_t clr, red, green, blue;

  led(true);      // turn on LED

  delay(60);  // takes 50ms to read 
  
  tcs.getRawData(&red, &green, &blue, &clr);

  led(false);  // turn off LED

  _red = red;
  _green = green;
  _blue = blue;
  _clr = clr;
  
//  Serial.print("C:\t"); Serial.print(clr);
//  Serial.print("\tR:\t"); Serial.print(red);
//  Serial.print("\tG:\t"); Serial.print(green);
//  Serial.print("\tB:\t"); Serial.print(blue);

  // Figure out some basic hex code for visualization
  //uint32_t sum = clr;
  //float r, g, b;
  //r = red; r /= sum;
  //g = green; g /= sum;
  //b = blue; b /= sum;
  //r *= 256; g *= 256; b *= 256;
  //Serial.print("\t");
  //Serial.print((int)r, HEX); Serial.print((int)g, HEX); Serial.print((int)b, HEX);
  //Serial.println();

}

int ColorSensor::getRed() {
  return _red;
}
int ColorSensor::getGreen() {
  return _green;
}
int ColorSensor::getBlue() {
  return _blue;
}
int ColorSensor::getClear() {
  return _clr;
}

