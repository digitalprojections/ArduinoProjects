/*
  FifteenTilePuzzle.h - Library for the game.
  Created by Ahmad Fuzal. February 25, 2024.
  Released into the public domain.
*/
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>
#ifndef FifteenTileGame_h
#define FifteenTileGame_h

#include "Arduino.h"

void Init();
void FTGLoop();
int indexOf(int value);
void generateDistinctRandomNumbers();
bool isSolvable();
bool isNumberAlreadyInArray(int num, int endIndex);
void drawSquareWithNumber();
void setNumbers(int sn, int dn);
bool LeftMove(int zero);
bool RightMove(int zero);
bool UpMove(int zero);
bool DownMove(int zero);
int checkInput();
void _endGame();
void endGame();

#endif