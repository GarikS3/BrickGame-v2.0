#include "backend.h"

void initStartState() {
  srand(getCurrentTime());
  GameState_t *state = getCurrentState();

  state->status = Initial;
  state->field = createMatrix(TETRIS_FIELD_H, TETRIS_FIELD_W);
  state->next_figure = generateNewFigure();
  state->figure = createMatrix(FIGURE_SIZE, FIGURE_SIZE);
  copyMatrix(state->figure, state->next_figure, FIGURE_SIZE, FIGURE_SIZE);

  state->fig_x = 4;
  state->fig_y = -4;
  state->score = 0;
  state->high_score = readHighScore();
  state->level = 1;
  state->speed = TETRIS_INIT_SPEED;
  state->pause = GameStart;
  state->time = getCurrentTime();
}

GameInfo_t updateCurrentState() {
  GameInfo_t info;
  GameState_t *state = getCurrentState();

  info.field = createMatrix(TETRIS_FIELD_H, TETRIS_FIELD_W);
  copyMatrix(info.field, state->field, TETRIS_FIELD_H, TETRIS_FIELD_W);

  info.next = createMatrix(FIGURE_SIZE, FIGURE_SIZE);
  copyMatrix(info.next, state->next_figure, FIGURE_SIZE, FIGURE_SIZE);

  if (info.field != NULL) updateGameField(info.field);
  info.score = state->score;
  info.high_score = state->high_score;
  info.level = state->level;
  info.speed = state->speed;
  info.pause = state->pause;

  return info;
}

void updateGameField(int **field) {
  GameState_t *state = getCurrentState();

  for (int i = 0; i < FIGURE_SIZE; i++) {
    for (int j = 0; j < FIGURE_SIZE; j++) {
      int x = state->fig_x + j;
      int y = state->fig_y + i;

      if (state->figure[i][j] == 1 && y > -1 && y < TETRIS_FIELD_H && x > -1 &&
          x < TETRIS_FIELD_W)
        field[y][x] = 1;
    }
  }
}

GameState_t *getCurrentState() {
  static GameState_t state;
  return &state;
}

void userInput(UserAction_t action, bool hold) {
  (void)hold;
  GameState_t *state = getCurrentState();
  bool can_control = (state->status == Moving && state->pause != GamePaused);

  switch (action) {
    case Start:
      if (state->status == Initial || state->status == GameOver) {
        initStartState();
        startGame();
      }
      break;
    case Pause:
      if (state->status != Initial) pauseGame();
      break;
    case Left:
      if (can_control) moveFigureLeft();
      break;
    case Right:
      if (can_control) moveFigureRight();
      break;
    case Down:
      if (can_control) shiftFigureDown();
      break;
    case Action:
      if (can_control) rotateFigure();
      break;
    case Terminate:
      finishGame();
      break;
    default:
      break;
  }
}

void handleGameState() {
  GameState_t *state = getCurrentState();

  switch (state->status) {
    case Spawn:
      spawnFigure();
      break;
    case Moving:
      if (timer(state->speed) && state->pause != GamePaused)
        state->status = Shifting;
      break;
    case Shifting:
      shiftFigureDown();
      break;
    case Attaching:
      attachFigureOnField();
      break;
    case GameOver:
      handleGameoverState();
      break;
    default:
      break;
  }
}

void startGame() {
  GameState_t *state = getCurrentState();
  state->status = Spawn;
  state->pause = GameON;
}

void pauseGame() {
  GameState_t *state = getCurrentState();
  if (state->pause == GameON)
    state->pause = GamePaused;
  else if (state->pause == GamePaused)
    state->pause = GameON;
}

void handleGameoverState() {
  GameState_t *state = getCurrentState();
  if (state->pause != GameTerminated) {
    state->pause = GameFinished;
  }
}

void finishGame() {
  GameState_t *state = getCurrentState();
  state->pause = GameTerminated;
}