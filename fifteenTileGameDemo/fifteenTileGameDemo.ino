#include "Adafruit.h"
#include "FifteenTileGame.h"
#include <EEPROM.h>

int highestScore;

int currentGame = -1;

//extern int leftBtn;
//extern int upBtn;
//extern int downBtn;
//extern int rightBtn;
extern int resetGameBtn;
extern int stopGameBtn;
extern int jsxInput;
extern int jsyInput;

extern unsigned long startTime;
extern unsigned long elapsedTime;

extern int inputNumber;
extern bool gameOver;

extern Adafruit_ST7789 tft;


void setup(void) {
  Serial.begin(9600);

  // Use this initializer (uncomment) if using a 1.3" or 1.54" 240x240 TFT:
  tft.init(240, 240, SPI_MODE3);  // Init ST7789 240x240
  tft.setRotation(2);

  // SPI speed defaults to SPI_DEFAULT_FREQ defined in the library, you can override it here
  // Note that speed allowable depends on chip and quality of wiring, if you go too fast, you
  // may end up with a black screen some times, or all the time.
  //tft.setSPISpeed(40000000);
  Serial.println(F("Initialized"));

  initializeButtons();

  // tft print function!
}

void loop() {

  // highestScore = EEPROM.read(0);
  // if (highestScore == 100) {
  //   Serial.print(EEPROM.length());
  //   EEPROM.write(0, 200);
  // }else
  // {
  //   EEPROM.write(0, 100);
  // }

  //Serial.println(currentGame);
  FTGLoop();
}


void initializeButtons() {

  pinMode(resetGameBtn, INPUT);
  pinMode(stopGameBtn, INPUT);
  pinMode(jsxInput, INPUT);
  pinMode(jsyInput, INPUT);
}
