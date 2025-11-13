#include "model.h"

namespace s21 {

SnakeModel::SnakeModel() : field_(nullptr) {
  initStartState();
  last_update_ = std::chrono::steady_clock::now();
}

SnakeModel::~SnakeModel() { deleteField(); }

void SnakeModel::initStartState() {
  std::srand(std::time(nullptr));

  deleteField();
  field_ = createField();
  direction_ = Direction::RIGHT;
  next_direction_ = Direction::RIGHT;
  score_ = 0;
  highScore_ = readHighScore();
  level_ = 1;
  speed_ = SNAKE_INIT_SPEED;
  status_ = INITIAL;
  boost_ = false;

  initSnake();
  generateNewApple();
}

void SnakeModel::initSnake() {
  snake_.clear();

  for (int i = 0; i < INIT_SNAKE_SIZE; ++i) {
    snake_.push_back(Point{INIT_SNAKE_SIZE - i - 2, SNAKE_FIELD_H / 2});
  }
}

void SnakeModel::generateNewApple() {
  std::vector<Point> free_cells;

  for (int y = 0; y < SNAKE_FIELD_H; ++y) {
    for (int x = 0; x < SNAKE_FIELD_W; ++x) {
      bool busy = false;
      for (auto &segment : snake_) {
        if (segment.x == x && segment.y == y) {
          busy = true;
          break;
        }
      }
      if (!busy) free_cells.push_back({x, y});
    }
  }
  if (!free_cells.empty()) {
    int index = rand() % free_cells.size();
    apple_ = free_cells[index];
  } else {
    status_ = WIN;
  }
}

void SnakeModel::changeDirection(Direction newDirection) {
  if ((newDirection == UP && direction_ != DOWN) ||
      (newDirection == DOWN && direction_ != UP) ||
      (newDirection == LEFT && direction_ != RIGHT) ||
      (newDirection == RIGHT && direction_ != LEFT)) {
    next_direction_ = newDirection;
  }
}

void SnakeModel::moveSnake() {
  direction_ = next_direction_;
  Point new_head = snake_.front();

  switch (direction_) {
    case LEFT:
      new_head.x--;
      break;
    case RIGHT:
      new_head.x++;
      break;
    case UP:
      new_head.y--;
      break;
    case DOWN:
      new_head.y++;
      break;
  }

  if (!checkCollision(new_head)) {
    snake_.insert(snake_.begin(), new_head);
    checkEatingApple(new_head);
  } else {
    finishGame();
  }
}

bool SnakeModel::checkCollision(const Point &head) {
  bool collision = false;

  if (head.x < 0 || head.x >= SNAKE_FIELD_W || head.y < 0 ||
      head.y >= SNAKE_FIELD_H)
    collision = true;

  for (size_t i = 1; i < snake_.size(); i++) {
    if (head.x == snake_[i].x && head.y == snake_[i].y) collision = true;
  }

  return collision;
}

void SnakeModel::checkEatingApple(const Point &head) {
  if (head.x == apple_.x && head.y == apple_.y) {
    score_ += 1;

    if (score_ > highScore_) {
      highScore_ = score_;
      saveHighScore(highScore_);
    }
    updateLevelAndSpeed();
    generateNewApple();

  } else {
    snake_.pop_back();
  }
}

void SnakeModel::startGame() { status_ = PLAYING; }

void SnakeModel::pauseGame() {
  if (status_ == PLAYING)
    status_ = PAUSED;
  else if (status_ == PAUSED)
    status_ = PLAYING;
}

void SnakeModel::terminateGame() { status_ = TERMINATED; }

void SnakeModel::finishGame() { status_ = GAMEOVER; }

void SnakeModel::updateLevelAndSpeed() {
  int new_level = score_ / SNAKE_MAX_SCORE_IN_LEVEL + 1;
  if (new_level > SNAKE_MAX_LEVEL) new_level = SNAKE_MAX_LEVEL;

  level_ = new_level;
  speed_ = SNAKE_INIT_SPEED - (level_ - 1) * SNAKE_SPEED_STEP;
}

void SnakeModel::increaseSpeed(bool active) { boost_ = active; }

void SnakeModel::updateGameField() {
  clearField();

  for (size_t i = 0; i < snake_.size(); ++i) {
    Point segment = snake_[i];
    if (segment.y >= 0 && segment.y < SNAKE_FIELD_H && segment.x >= 0 &&
        segment.x < SNAKE_FIELD_W)
      field_[segment.y][segment.x] =
          (i == 0) ? CELL_SNAKE_HEAD : CELL_SNAKE_BODY;
  }
  if (apple_.y >= 0 && apple_.y < SNAKE_FIELD_H && apple_.x >= 0 &&
      apple_.x < SNAKE_FIELD_W)
    field_[apple_.y][apple_.x] = CELL_APPLE;
}

GameInfo_t SnakeModel::getGameState() {
  if (status_ == PLAYING) {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                       now - last_update_)
                       .count();

    int current_speed = boost_ ? BOOST_SPEED : speed_;
    if (elapsed >= current_speed) {
      last_update_ = now;
      moveSnake();
      updateGameField();
    }
  }

  GameInfo_t info;
  info.field = field_;
  info.next = nullptr;
  info.score = score_;
  info.high_score = highScore_;
  info.level = level_;
  info.speed = speed_;
  info.pause = status_;

  return info;
}

int SnakeModel::readHighScore() {
  int high_score = 0;
  std::ifstream inputFile("snake_high_score.txt");

  if (inputFile.is_open()) {
    inputFile >> high_score;
  }
  return high_score;
}

void SnakeModel::saveHighScore(int score) {
  std::ofstream outputFile("snake_high_score.txt");

  if (outputFile.is_open()) {
    outputFile << score;
  }
}

int **SnakeModel::createField() {
  field_ = new int *[SNAKE_FIELD_H];
  for (int i = 0; i < SNAKE_FIELD_H; ++i) {
    field_[i] = new int[SNAKE_FIELD_W]{0};
  }
  return field_;
}

void SnakeModel::clearField() {
  if (field_) {
    for (int y = 0; y < SNAKE_FIELD_H; ++y) {
      for (int x = 0; x < SNAKE_FIELD_W; ++x) {
        field_[y][x] = CELL_EMPTY;
      }
    }
  }
}

void SnakeModel::deleteField() {
  if (field_) {
    for (int i = 0; i < SNAKE_FIELD_H; ++i) {
      delete[] field_[i];
    }
    delete[] field_;
    field_ = nullptr;
  }
}

int SnakeModel::getGameStatus() { return status_; }
}  // namespace s21