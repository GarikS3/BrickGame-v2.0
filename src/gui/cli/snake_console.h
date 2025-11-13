#pragma once

#include <ncurses.h>

#include "../../brick_game/snake/controller/controller.h"

namespace s21 {

#define SNAKE_WINDOW_H (SNAKE_FIELD_H + 2)
#define SNAKE_WINDOW_W (SNAKE_FIELD_W * 3 + 2)
#define WINDOW_START_Y 1
#define WINDOW_START_X 1

#define START_BUTTON 115
#define PAUSE_BUTTON 112
#define ACTION_BUTTON 32
#define TERMINATE_BUTTON 113

void initNcurses();
void setColors();
void snakeGameLoop();
UserAction_t processInput(int ch);
void renderGame(GameInfo_t info);
WINDOW *drawGameWindow(GameInfo_t info);
WINDOW *drawInfoWindow(GameInfo_t info);
void drawStartPanel();
void drawPausePanel();
void drawGameoverPanel();

}  // namespace s21