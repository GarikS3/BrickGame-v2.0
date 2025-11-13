#pragma once

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

namespace s21 {

constexpr int SNAKE_FIELD_W = 10;
constexpr int SNAKE_FIELD_H = 20;

constexpr int INIT_SNAKE_SIZE = 4;
constexpr int SNAKE_MAX_LEVEL = 10;
constexpr int SNAKE_MAX_SCORE_IN_LEVEL = 5;
constexpr int SNAKE_INIT_SPEED = 600;
constexpr int SNAKE_SPEED_STEP = 50;
constexpr int BOOST_SPEED = 100;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef enum { INITIAL, PLAYING, PAUSED, TERMINATED, WIN, GAMEOVER } GameStatus;

typedef enum { LEFT, RIGHT, UP, DOWN } Direction;

typedef enum {
  CELL_EMPTY,
  CELL_SNAKE_HEAD,
  CELL_SNAKE_BODY,
  CELL_APPLE
} CellType;

struct Point {
  int x, y;
};

class SnakeModel {
 public:
  SnakeModel();
  ~SnakeModel();

  GameInfo_t getGameState();
  void initStartState();
  void startGame();
  void pauseGame();
  void terminateGame();
  void changeDirection(Direction newDirection);
  int getGameStatus();
  void initSnake();
  void generateNewApple();
  void moveSnake();
  void updateGameField();
  bool checkCollision(const Point &head);
  void checkEatingApple(const Point &head);
  void finishGame();
  void updateLevelAndSpeed();
  void increaseSpeed(bool active);
  int readHighScore();
  void saveHighScore(int score);
  int **createField();
  void clearField();
  void deleteField();

  int **field_;
  std::vector<Point> snake_;
  Point apple_;
  Direction direction_;
  Direction next_direction_;
  int score_;
  int highScore_;
  int level_;
  int speed_;
  int status_;
  bool boost_;
  std::chrono::steady_clock::time_point last_update_;
};

}  // namespace s21