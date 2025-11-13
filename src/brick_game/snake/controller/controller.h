#pragma once

#include "../model/model.h"

namespace s21 {

class SnakeController {
 public:
  SnakeController();

  void userInput(UserAction_t action, bool hold);
  GameInfo_t updateCurrentState();

 private:
  SnakeModel model_;
};

}  // namespace s21