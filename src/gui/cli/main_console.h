#pragma once

#include "snake_console.h"
#include "tetris_console.h"

#define MENU_ITEMS_SIZE 3

const char *items[MENU_ITEMS_SIZE] = {"Tetris", "Snake", "Exit"};

void drawMenu(int selectedItem);
void startTetris();
void startSnake();