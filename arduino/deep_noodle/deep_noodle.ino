
/********************************************************************
 *   Deep Noodle -- An OLED thought bubble for NoodleFeet's brain.
 *                 http://noodlefeet.zoness.com
 *   
 *   
 *   Requires SSD1306 library (for OLED display) from Adafruit 
 *   but is not redistributed here.
 *                 http://www.adafruit.com/category/63_98
 *
 ********************************************************************/

#define VER_MAJ 0
#define VER_MIN 1

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000011, B11000000,
  B00000100, B00100000,
  B00001010, B01010000,
  B00001000, B00010000,
  B00001111, B11110000,
  B00000001, B10000000,
  B00000001, B10000000,
  B00000001, B10000000,
  B01101101, B10110110,
  B10010011, B11001001,
  B10010011, B11001001,
  B10010011, B11001001,
  B10010010, B01001001,
  B10010010, B01001001,
  B01110010, B01001110,
  B00011100, B00111000
 };


static const char *thought[] = {
  "pintos are wet .",
  "avocados are \ngreen butter .",
  "butter .",
  "pasta is nice hard .",
  "mash the beans .",
  "smooth pieces \nare dirty .",
  "press fit the socks .",
  "pizza warms \nthe feet !",
  "socks come in \ncolors . \nflavors . \ntastes .",
  "my favorite is \numami .",
  "mommy is love .",
//  "brain daddy likes \nred juice .",
//  "shape daddy tastes \nlike pennies .",
  "art \nis what happens after .",
//  "jello .",
  "jello is wet shaped .",
  "looks like candy .",
  "favorite color = lint .",
//  "dryers are soft \ndistilleries .",
  "distilled softs \nare lints .",
  "i like lint =)",
//  "no rushing \nwhile popping .",
  "pop one bubble\n at a time . \nenjoy .",
  "mommy is \nmaking me\n tongue buttons .",
  "spoons.",
  "i own mars .",
//  "i want to be alone .",
  "i want to feel\ncarpet .",
  "mars is\na\ncarpet party .",
  "my favorite flavor \nis pink .",
  "pink is like lemons .",
  "cuddle\nin the softs .",
  "nest\nwith the deltas .\nmust keep them safe .",
  "protect .",
  "i have many nuts .",
  "look at my pizza .",
  "put your toes \nin the salmon .",
  "i will put the toe \nin tokyo .",
//  "shape daddy \nhelps mommy \nmake the mental pieces .",
//  "brain daddy melts \nthe tiny bits to \nthe smart pieces .",
  "carl snores .",
  "i like\nlight candies .",
  "i want to\ntap dance .",
  "spaghetti\nis cannibalism .",
//  "toast .",
//  "i have creeper feet .",
//  "i have 0 GPUs . \nbut I have 4 TPUs .",
//  "taste processing unit .",
//  "i want to go \nhide in the yellows .",
//  "who are you ?",
//  "i like spam .",
  "doughnuts are \nfeet pillows with \ncandy coating .",
  "don't swallow \nthe bottle cap . \nIt will get stuck .",
  "i am not a \npez dispenser .",
  "i'm not good at \nwalking .",
  "beep is love .",
//  "beep.",
//  "Beeeeeeeeeeeeeeeep.",
//  "aaa",
 // "Aaaaaaaaaaaaaaaaaa.",
//  "awsd.",
 // "stop looking at me .",
 // "bring me a candy .",
 // "i want a shape .",
  "i am not a balloon .",
//  "i am not \na fleshlight ."
  };

#define nThoughts (sizeof(thought)/sizeof(char *)) //array size  

void setup()   {                
  //Serial.begin(115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  


  // TODO:  LEDs

  
  // Show image buffer on the display hardware.
  // First time, this will display the Adafruit splashscreen.
  display.display();
  delay(1000);
  display.clearDisplay();
 
  showNoodleBanner();
  delay(2000);
  display.clearDisplay();
   
}

void loop() {
  // Display noodle messages
  int rand = random(0,nThoughts);
  display.setCursor(0,0);
  display.println(thought[rand]);
  
  display.drawBitmap(110, 16,  logo16_glcd_bmp, 16, 16, 1);
  display.display();
  delay(10000);
  display.clearDisplay();
 
  
}


void showNoodleBanner() {
  display.drawBitmap(12, 16,  logo16_glcd_bmp, 16, 16, 1);
  
  display.setCursor(-5,0);
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.setTextSize(2);
  display.println(" DeepNoodle");
  
  
  char ver[5];
  sprintf( ver, "%i.%i", VER_MAJ, VER_MIN);
  
  display.setCursor(100,21);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);
  display.print("v");display.println(ver);
  
  display.display();
}

void demoScreens() {

  // draw scrolling text
  testscrolltext();
  delay(2000);
  display.clearDisplay();

  // miniature bitmap display
  display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
  display.display();
  delay(1);

  // invert the display
  display.invertDisplay(true);
  delay(1000); 
  display.invertDisplay(false);
  delay(1000); 
  display.clearDisplay();

}


void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("scroll");
  display.display();
  delay(1);
 
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);    
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
}
