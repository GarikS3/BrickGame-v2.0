#include "main_console.h"

int main() {
  initNcurses();
  int ch, selectedItem = 0, exit = 0;

  while (!exit) {
    timeout(10);

    drawMenu(selectedItem);
    ch = getch();

    switch (ch) {
      case KEY_UP:
        selectedItem =
            (selectedItem == 0) ? MENU_ITEMS_SIZE - 1 : selectedItem - 1;
        break;
      case KEY_DOWN:
        selectedItem =
            (selectedItem == MENU_ITEMS_SIZE - 1) ? 0 : selectedItem + 1;
        break;
      case '\n':
        if (selectedItem == 0)
          startTetris();
        else if (selectedItem == 1)
          startSnake();
        else if (selectedItem == 2)
          exit = 1;
        break;
    }
  }
  endwin();

  return 0;
}

void drawMenu(int selectedItem) {
  clear();

  mvprintw(1, 1, "------------------------------");
  mvprintw(2, 5, "-=BrickGame Menu=-");

  for (int i = 0; i < MENU_ITEMS_SIZE; i++) {
    if (i == selectedItem) {
      attron(A_REVERSE);
      mvprintw(4 + i * 2, 11, "%s", items[i]);
      attroff(A_REVERSE);
    } else {
      mvprintw(4 + i * 2, 11, "%s", items[i]);
    }
  }

  mvprintw(9, 1, "------------------------------");
  refresh();
}

void startTetris() { tetrisGameLoop(); }

void startSnake() { s21::snakeGameLoop(); }