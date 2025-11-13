#ifndef TETRIS_CONSOLE_H
#define TETRIS_CONSOLE_H

#include <ncurses.h>

#include "../../brick_game/tetris/backend.h"

#define WINDOW_H (TETRIS_FIELD_H + 2)
#define WINDOW_W (TETRIS_FIELD_W * 3 + 2)
#define WINDOW_START_Y 1
#define WINDOW_START_X 1

#define START_BUTTON 115
#define PAUSE_BUTTON 112
#define ACTION_BUTTON 32
#define TERMINATE_BUTTON 113

#ifdef __cplusplus
extern "C" {
#endif

void initNcurses();
void setColors();
void tetrisGameLoop();
UserAction_t processInput(int ch);
void renderGame(GameInfo_t info);
WINDOW *drawGameWindow(GameInfo_t info);
WINDOW *drawInfoWindow(GameInfo_t info);
void drawStartPanel();
void drawPausePanel();
void drawGameoverPanel();

#ifdef __cplusplus
}
#endif

#endif