#include "FifteenTileGame.h"
#include "Adafruit.h"

const String games[] = { "15 tile puzzle", "Battleship", "Ping-pong", "SpaceInvaders" };

int currentGame = -1;

extern int leftBtn;
extern int upBtn;
extern int downBtn;
extern int rightBtn;
extern int resetGameBtn;
extern int stopGameBtn;

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

  elapsedTime = millis() - startTime;

  inputNumber = checkInput();
  
  //Serial.println(F("menu"));
  //Serial.println(inputNumber);  
  if (gameOver) {
    currentGame = gameChoice();
    gameOver = false;
    switch (currentGame) {
      case 0:
      Serial.println(inputNumber);
        Init();
        break;
      case 1:
      Serial.println(inputNumber);
        BattleShipGame();
        break;
      case 2:
      Serial.println(inputNumber);
        PingPongGame();
        break;
      case 3:
      Serial.println(inputNumber);
        SpaceShipGame();
        break;
    }
  } else {
    switch (currentGame) {
      case 0:
      Serial.println(currentGame);
        FTGLoop();
        break;
      case 1:
      Serial.println(currentGame);
        BattleShipLoop();
        break;
      case 2:
      Serial.println(currentGame);
        PingPongLoop();
        break;
      case 3:
      //Serial.println(currentGame);
        SpaceShipLoop();
        break;
    }
  }
}

//these lines will be moved out to their own libraries
void PingPongGame() {
}
void PingPongLoop() {
}

void BattleShipGame() {
}
void BattleShipLoop() {
}



void initializeButtons() {

  pinMode(resetGameBtn, INPUT);
  pinMode(stopGameBtn, INPUT);
  pinMode(leftBtn, INPUT);
  pinMode(upBtn, INPUT);
  pinMode(downBtn, INPUT);
  pinMode(rightBtn, INPUT);
}



int gameChoice() {
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 30);
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(2);
  for (int i = 0; i < 4; i++) {
    tft.print(i);
    tft.print(" - ");
    tft.println(games[i]);
    delay(100);
  }

  bool gameselected = false;
  int tempinputnumber = -1;
  gameOver = false;

  while (!gameselected) {
    tempinputnumber = checkInput();
    elapsedTime = millis() - startTime;
  
    if (tempinputnumber >= 0 && tempinputnumber != 5 && elapsedTime > 200) {
      
      if(tempinputnumber == 4 && currentGame < 0)
    {
      return currentGame;
    }
      
      if (tempinputnumber == 4 && currentGame >= 0) {
        gameselected = true;
      } else {
        currentGame = tempinputnumber;
        startTime = millis();
        tft.setTextColor(ST77XX_GREEN);
        tft.setCursor(0, 130);
        tft.fillRect(0, 125, 240, 40, ST77XX_ORANGE);
        tft.println("Selected game:");
        tft.println(games[tempinputnumber]);
      }
    }
  }

  return currentGame;
}

void mediabuttons() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_CYAN);
  tft.println("Press Any Button");
  tft.println("To PLAY again");
}
