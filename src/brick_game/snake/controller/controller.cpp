#include "controller.h"

namespace s21 {

SnakeController::SnakeController() {}

void SnakeController::userInput(UserAction_t action, bool hold) {
  switch (action) {
    case Start:
      if (model_.getGameStatus() == INITIAL || model_.getGameStatus() == WIN ||
          model_.getGameStatus() == GAMEOVER) {
        model_.initStartState();
        model_.startGame();
      }
      break;
    case Pause:
      if (model_.getGameStatus() != INITIAL) model_.pauseGame();
      break;
    case Terminate:
      model_.terminateGame();
      break;
    case Left:
      if (model_.getGameStatus() == PLAYING) model_.changeDirection(LEFT);
      break;
    case Right:
      if (model_.getGameStatus() == PLAYING) model_.changeDirection(RIGHT);
      break;
    case Up:
      if (model_.getGameStatus() == PLAYING) model_.changeDirection(UP);
      break;
    case Down:
      if (model_.getGameStatus() == PLAYING) model_.changeDirection(DOWN);
      break;
    case Action:
      if (model_.getGameStatus() == PLAYING) model_.increaseSpeed(hold);
      break;

    default:
      break;
  }
}

GameInfo_t SnakeController::updateCurrentState() {
  return model_.getGameState();
  ;
}

}  // namespace s21