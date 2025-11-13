#include <gtest/gtest.h>

#include "../brick_game/snake/controller/controller.h"
#include "../brick_game/snake/model/model.h"

TEST(Snake, TestInit) {
  s21::SnakeModel model;
  EXPECT_EQ(model.direction_, s21::Direction::RIGHT);
  EXPECT_EQ(model.score_, 0);
  EXPECT_EQ(model.level_, 1);
  EXPECT_EQ(model.speed_, s21::SNAKE_INIT_SPEED);
  EXPECT_EQ(model.status_, s21::INITIAL);
  EXPECT_EQ(model.snake_.size(), static_cast<size_t>(s21::INIT_SNAKE_SIZE));
}

TEST(Snake, TestStart) {
  s21::SnakeModel model;
  model.startGame();
  EXPECT_EQ(model.getGameStatus(), s21::PLAYING);
}

TEST(Snake, TestPause) {
  s21::SnakeModel model;
  model.startGame();
  model.pauseGame();
  EXPECT_EQ(model.getGameStatus(), s21::PAUSED);
  model.pauseGame();
  EXPECT_EQ(model.getGameStatus(), s21::PLAYING);
}

TEST(Snake, TestTerminate) {
  s21::SnakeModel model;
  model.startGame();
  model.terminateGame();
  EXPECT_EQ(model.getGameStatus(), s21::TERMINATED);
}

TEST(Snake, TestWin) {
  s21::SnakeModel model;
  model.startGame();
  model.snake_.clear();
  for (int y = 0; y < s21::SNAKE_FIELD_H; ++y) {
    for (int x = 0; x < s21::SNAKE_FIELD_W; ++x) {
      model.snake_.push_back({x, y});
    }
  }
  auto head = model.snake_.front();
  model.apple_ = head;
  model.checkEatingApple(head);
  EXPECT_EQ(model.getGameStatus(), s21::WIN);
}

TEST(Snake, TestChangeDirection) {
  s21::SnakeModel model;
  model.changeDirection(s21::Direction::LEFT);
  EXPECT_EQ(model.next_direction_, s21::Direction::RIGHT);

  model.changeDirection(s21::Direction::DOWN);
  EXPECT_EQ(model.next_direction_, s21::Direction::DOWN);
  model.moveSnake();

  model.changeDirection(s21::Direction::LEFT);
  EXPECT_EQ(model.next_direction_, s21::Direction::LEFT);
  model.moveSnake();

  model.changeDirection(s21::Direction::UP);
  EXPECT_EQ(model.next_direction_, s21::Direction::UP);
}

TEST(Snake, TestGenerateApple) {
  s21::SnakeModel model;
  model.startGame();
  model.snake_.clear();
  for (int y = 0; y < s21::SNAKE_FIELD_H; ++y) {
    for (int x = 0; x < s21::SNAKE_FIELD_W; ++x) {
      if (!(x == 0 && y == 0)) model.snake_.push_back({x, y});
    }
  }
  model.generateNewApple();
  EXPECT_EQ(model.apple_.x, 0);
  EXPECT_EQ(model.apple_.y, 0);
}

TEST(Snake, TestEatingApple) {
  s21::SnakeModel model;
  model.startGame();
  auto head = model.snake_.front();
  model.apple_ = {head.x + 1, head.y};
  model.moveSnake();
  EXPECT_EQ(model.score_, 1);
  EXPECT_EQ(model.snake_.size(), static_cast<size_t>(s21::INIT_SNAKE_SIZE + 1));
}

TEST(Snake, TestCollisionWithWall) {
  s21::SnakeModel model;
  model.startGame();
  model.snake_.clear();
  model.snake_.push_back({s21::SNAKE_FIELD_W - 1, 2});
  model.direction_ = s21::Direction::RIGHT;
  model.moveSnake();
  EXPECT_EQ(model.getGameStatus(), s21::GAMEOVER);
}

TEST(Snake, TestCollisionWithSelf) {
  s21::SnakeModel model;
  model.startGame();
  model.snake_.clear();
  model.snake_.push_back({5, 5});
  model.snake_.push_back({4, 5});
  model.snake_.push_back({4, 4});
  model.snake_.push_back({5, 4});
  model.snake_.push_back({6, 4});
  model.direction_ = s21::Direction::UP;
  model.next_direction_ = s21::Direction::UP;
  model.moveSnake();
  EXPECT_EQ(model.getGameStatus(), s21::GAMEOVER);
}

TEST(Snake, TestUpdateLevelAndSpeed) {
  s21::SnakeModel model;
  model.startGame();
  model.score_ = s21::SNAKE_MAX_SCORE_IN_LEVEL;
  model.updateLevelAndSpeed();
  EXPECT_EQ(model.level_, 2);
  EXPECT_EQ(model.speed_, s21::SNAKE_INIT_SPEED - s21::SNAKE_SPEED_STEP);
}

TEST(Snake, TestMaxLevel) {
  s21::SnakeModel model;
  model.startGame();
  model.score_ = 1000;
  model.updateLevelAndSpeed();
  EXPECT_EQ(model.level_, s21::SNAKE_MAX_LEVEL);
}

TEST(Snake, TestReadHighScore) {
  s21::SnakeModel model;
  model.startGame();
  model.saveHighScore(100);
  EXPECT_EQ(model.readHighScore(), 100);
}

TEST(Snake, TestClearField) {
  s21::SnakeModel model;
  model.startGame();
  model.field_[0][0] = s21::CELL_SNAKE_HEAD;
  model.clearField();
  for (int y = 0; y < s21::SNAKE_FIELD_H; ++y) {
    for (int x = 0; x < s21::SNAKE_FIELD_W; ++x) {
      EXPECT_EQ(model.field_[y][x], s21::CELL_EMPTY);
    }
  }
}

TEST(Snake, TestDeleteField) {
  s21::SnakeModel model;
  model.deleteField();
  EXPECT_EQ(model.field_, nullptr);
}

TEST(SnakeController, TestStart) {
  s21::SnakeController controller;
  controller.userInput(s21::Start, false);
  EXPECT_EQ(controller.updateCurrentState().pause, s21::PLAYING);
}

TEST(SnakeController, TestPause) {
  s21::SnakeController controller;
  controller.userInput(s21::Start, false);
  controller.userInput(s21::Pause, false);
  EXPECT_EQ(controller.updateCurrentState().pause, s21::PAUSED);
  controller.userInput(s21::Pause, false);
  EXPECT_EQ(controller.updateCurrentState().pause, s21::PLAYING);
}

TEST(SnakeController, TestTerminate) {
  s21::SnakeController controller;
  controller.userInput(s21::Start, false);
  controller.userInput(s21::Terminate, false);
  EXPECT_EQ(controller.updateCurrentState().pause, s21::TERMINATED);
}