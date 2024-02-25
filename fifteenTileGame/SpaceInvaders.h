/*
  SpaceInvaders.h - Library for the game.
  Created by Ahmad Fuzal. February 25, 2024.
  Released into the public domain.
*/

void redrawGame(uint16_t n);
void SpaceShipLoop();
void SpaceShipGame();
bool LeftMove();
bool RightMove();
bool UpMove();
bool DownMove();
void spawnSpaceJet(uint16_t x, uint16_t y);