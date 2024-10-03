/*
  PingPong.h - Library for the game.
  Created by Ahmad Fuzal. October 3, 2024.
  Released into the public domain.
*/

void PingPongGame();
void PingPongLoop();

void PP_redrawGame(int n);
bool PP_LeftMove();
bool PP_RightMove();
bool PP_UpMove();
bool PP_DownMove();
void drawRacket(int x);