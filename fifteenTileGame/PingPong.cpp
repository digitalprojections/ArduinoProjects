#include "HardwareSerial.h"
#include "Adafruit_ST77xx.h"
#include "Arduino.h"
#include "Adafruit.h"
#include "PingPong.h"

extern Adafruit_ST7789 tft;

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

//Game variables
uint16_t rw = 40;
uint16_t rh = 5;
unsigned long ppGameStartTime;
unsigned long ballMoveTime;
unsigned long racketMoveTime;


uint16_t rx = 105;  //racket origin
uint16_t ry = 235;

void PingPongGame() {
  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);
  tft.setCursor(80, 30);
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.setTextWrap(true);
  tft.println("Ping Pong");

  delay(1000);

  ppGameStartTime = millis();

  //clear screen
  tft.fillRect(0, 0, 240, 240, ST77XX_BLACK);

  //initial draw. once
  drawRacket(0);
}

//game`s main loop
void PingPongLoop() {
  bool goodMove = false;
  inputNumber = checkInput();


  //gameover check
  if (gameOver && inputNumber >= 0) {
    gameOver = false;

    PingPongGame();
  }
  elapsedTime = millis() - ppGameStartTime;

  //if the input a direction button
  if (inputNumber >= 0 && elapsedTime > 5) {
    ppGameStartTime = millis();

    switch (inputNumber) {
      case 0:  //Left
        goodMove = PP_LeftMove();
        break;
      case 1:  //Up, not supported
        goodMove = PP_UpMove();
        break;
      case 2:  //Down. not supported
        goodMove = PP_DownMove();
        break;
      case 3:  //Right
        goodMove = PP_RightMove();
        break;
      case 4:  //reset
        gameOver = false;
        break;
      case 5:
        gameOver = true;
        rx = 100;
        break;
      default:
        break;
    }

    if (goodMove) {
      PP_redrawGame(inputNumber);
    } else {
      IllegalMove();
    }

    inputNumber = -1;
  }
}

//MOVES

//Left
bool PP_LeftMove() {
  return true;
}

//Right
bool PP_RightMove() {
  return true;
}
//Upwards
bool PP_UpMove() {
  return false;
}
//Down
bool PP_DownMove() {
  return false;
}

//draw racket
void drawRacket(int x) {

  //redraw the racket
    //clear the section
    tft.fillRect(rx, ry, rw, rh, ST77XX_BLACK);
    rx = rx + x;
    tft.fillRect(rx, ry, rw, rh, ST77XX_WHITE);

  if (rx > 65000) {
    rx = 0;
  } 
  Serial.println(rx);

  if(rx > 220)
  {
    rx = 220;
  }
}


//Called to refresh the screen
void PP_redrawGame(int move_x = 0) {

  if (move_x == 0) {
    //left move
    drawRacket(-1);
  } else if (move_x == 3) {
    //right move
    drawRacket(1);
  } else if (move_x == 4) {
    Serial.print("move_x: ");
    Serial.println(move_x);
  }
}