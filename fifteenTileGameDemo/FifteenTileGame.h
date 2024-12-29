/*
  FifteenTilePuzzle.h - Library for the game.
  Created by Ahmad Fuzal. February 25, 2024.
  Released into the public domain.
*/

#ifndef FifteenTileGame_h
#define FifteenTileGame_h

#include "Arduino.h"

void FifteenTileGame();
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
void endGame();
int checkInput();

#endif