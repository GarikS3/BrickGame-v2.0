#include "snake_console.h"

namespace s21 {

void snakeGameLoop() {
  SnakeController controller_;
  initNcurses();
  int ch;
  bool hold;

  while (true) {
    timeout(10);
    ch = getch();
    UserAction_t action = processInput(ch);
    hold = false;

    if (action == Action) hold = true;

    if (ch == ERR)
      controller_.userInput(Action, false);
    else
      controller_.userInput(action, hold);

    GameInfo_t info = controller_.updateCurrentState();
    renderGame(info);

    if (info.pause == TERMINATED) {
      break;
    }
  }

  endwin();
}

void initNcurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, 1);
  nodelay(stdscr, 1);
  curs_set(0);
  setColors();
}

void setColors() {
  start_color();
  init_pair(1, COLOR_CYAN, COLOR_BLACK);
  init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_GREEN, COLOR_BLACK);
  init_pair(5, COLOR_RED, COLOR_BLACK);
  init_pair(6, COLOR_BLACK, COLOR_BLACK);
}

void renderGame(GameInfo_t info) {
  refresh();

  WINDOW *game_window = NULL;
  if (info.pause != INITIAL && info.pause != PAUSED) {
    game_window = drawGameWindow(info);
    wrefresh(game_window);
  }

  WINDOW *info_window = drawInfoWindow(info);
  wrefresh(info_window);

  switch (info.pause) {
    case INITIAL:
      drawStartPanel();
      break;
    case PAUSED:
      drawPausePanel();
      break;
    case GAMEOVER:
      drawGameoverPanel();
      break;
  }

  if (game_window != NULL) delwin(game_window);
  delwin(info_window);
}

WINDOW *drawGameWindow(GameInfo_t info) {
  WINDOW *game_window =
      newwin(SNAKE_WINDOW_H, SNAKE_WINDOW_W, WINDOW_START_Y, WINDOW_START_X);
  wattron(game_window, COLOR_PAIR(1));
  box(game_window, 0, 0);

  for (int i = 0; i < SNAKE_FIELD_H; i++) {
    for (int j = 0; j < SNAKE_FIELD_W; j++) {
      switch (info.field[i][j]) {
        case CELL_SNAKE_HEAD:
          wattron(game_window, COLOR_PAIR(4));
          mvwprintw(game_window, i + 1, 3 * j + 1, "[ ]");
          break;
        case CELL_SNAKE_BODY:
          wattron(game_window, COLOR_PAIR(3));
          mvwprintw(game_window, i + 1, 3 * j + 1, "[ ]");
          break;
        case CELL_APPLE:
          wattron(game_window, COLOR_PAIR(5));
          mvwprintw(game_window, i + 1, 3 * j + 1, "[ ]");
          break;
        default:
          wattron(game_window, COLOR_PAIR(1));
          mvwprintw(game_window, i + 1, 3 * j + 1, "   ");
          break;
      }
    }
  }
  return game_window;
}

WINDOW *drawInfoWindow(GameInfo_t info) {
  WINDOW *info_window = newwin(SNAKE_WINDOW_H, SNAKE_WINDOW_W, WINDOW_START_Y,
                               WINDOW_START_X + SNAKE_WINDOW_W + 2);
  wattron(info_window, COLOR_PAIR(1));
  box(info_window, 0, 0);

  wattron(info_window, COLOR_PAIR(1));
  mvwprintw(info_window, 2, 5, "SCORE:  %d", info.score);
  mvwprintw(info_window, 5, 5, "RECORD:  %d", info.high_score);
  mvwprintw(info_window, 8, 5, "LEVEL:  %d", info.level);

  wattron(info_window, COLOR_PAIR(2));
  mvwprintw(info_window, 15, 3, "Control: Up Down Left Right");
  mvwprintw(info_window, 17, 3, "Pause: 'p'");
  mvwprintw(info_window, 19, 3, "Exit game: 'q'");

  return info_window;
}

void drawStartPanel() {
  WINDOW *start_window =
      newwin(SNAKE_WINDOW_H, SNAKE_WINDOW_W, WINDOW_START_Y, WINDOW_START_X);
  wattron(start_window, COLOR_PAIR(1));
  box(start_window, 0, 0);
  wattron(start_window, COLOR_PAIR(2));
  mvwprintw(start_window, 6, 6, "____________________");
  mvwprintw(start_window, 9, 13, "SNAKE");
  mvwprintw(start_window, 11, 6, "____________________");
  mvwprintw(start_window, 17, 7, "Press 's' to start");
  wrefresh(start_window);
  delwin(start_window);
}

void drawPausePanel() {
  WINDOW *pause_window = newwin(5, 15, 9, 10);
  wattron(pause_window, COLOR_PAIR(2));
  box(pause_window, 0, 0);
  mvwprintw(pause_window, 2, 5, "PAUSE");
  wrefresh(pause_window);
  delwin(pause_window);
}

void drawGameoverPanel() {
  WINDOW *gameover_window = newwin(6, 22, 8, 6);
  wattron(gameover_window, COLOR_PAIR(2));
  box(gameover_window, 0, 0);
  mvwprintw(gameover_window, 1, 7, "GAME OVER");
  mvwprintw(gameover_window, 3, 4, "'s' - new game");
  mvwprintw(gameover_window, 4, 4, "'q' - quit");
  wrefresh(gameover_window);
  delwin(gameover_window);
}

UserAction_t processInput(int ch) {
  UserAction_t action = static_cast<UserAction_t>(-1);
  if (ch >= 0 && ch <= 255) ch = tolower((unsigned char)ch);

  switch (ch) {
    case START_BUTTON:
      action = Start;
      break;
    case PAUSE_BUTTON:
      action = Pause;
      break;
    case TERMINATE_BUTTON:
      action = Terminate;
      break;
    case KEY_LEFT:
      action = Left;
      break;
    case KEY_RIGHT:
      action = Right;
      break;
    case KEY_UP:
      action = Up;
      break;
    case KEY_DOWN:
      action = Down;
      break;
    case ACTION_BUTTON:
      action = Action;
      break;
  }

  return action;
}

}  // namespace s21