/*********************************************************************
 CO2 Sensor for Arduino Micro
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

SoftwareSerial softSerial(8,9); // RX, TX

byte inBuffer[9];
byte outBuffer[9];

void setup()   {                
  Serial.begin(9600);
  softSerial.begin(9600);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.display();
  delay(2000);
  display.clearDisplay();

  inBuffer[0] = byte(0xff);
  inBuffer[1] = byte(0x01);
  inBuffer[2] = byte(0x86);
  inBuffer[3] = byte(0x00);
  inBuffer[4] = byte(0x00);
  inBuffer[5] = byte(0x00);
  inBuffer[6] = byte(0x00);
  inBuffer[7] = byte(0x00);
  inBuffer[8] = byte(0x79);
}


void loop() {
  // Send gas consentration reading command
  softSerial.write(inBuffer, 9);

  for (int i=0; i<9; i++) {
    if(softSerial.available()) {
      outBuffer[i] = softSerial.read();
    }
  }
  
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);  
  display.println("CO2: ");

  if(outBuffer[0] == 0xff && outBuffer[1] == 0x86) {
    display.print(outBuffer[2] * 256 + outBuffer[3]);
    display.print(" ppm");
  } else {
    display.print("Unabilable");    
  }
  display.display();
  delay(1000);
  display.clearDisplay();  
}
