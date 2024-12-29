#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>
#include "Adafruit_ST77xx.h"
#include <Adafruit_PCF8574.h>



#define TFT_CS 8    // if your display has CS pin
#define TFT_RST 10  // reset pin
#define TFT_DC 9    // data pin
#define TFT_MOSI 11
#define TFT_SCLK 13

Adafruit_PCF8574 pcf;

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

void setup(void) {
  Serial.begin(9600);

  // Use this initializer (uncomment) if using a 1.3" or 1.54" 240x240 TFT:
  tft.init(240, 240, SPI_MODE3);  // Init ST7789 240x240
  tft.setRotation(2);

  Serial.println(F("Initialized"));
  // tft print function!
  tft.setTextSize(4);
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(20, 110);
  tft.fillRect(0, 105, 240, 40, ST77XX_ORANGE);
  tft.println("You Won!");

  if (!pcf.begin(0x20, &Wire)) {
    Serial.println("Couldn't find PCF8574");
    while (1);
  }
  pcf.pinMode(7, OUTPUT);
  Serial.println("Could find PCF8574");
}

void loop() {
  pcf.digitalWrite(7, LOW);  // turn LED on by sinking current to ground
  delay(100);
  pcf.digitalWrite(7, HIGH); // turn LED off by turning off sinking transistor
  delay(100);
}