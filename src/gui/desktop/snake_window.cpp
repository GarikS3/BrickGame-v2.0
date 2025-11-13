#include "snake_window.h"

namespace s21 {

SnakeWindow::SnakeWindow(QWidget *parent) : QWidget(parent) {
  int windowWidth = (SNAKE_FIELD_W * CELL_SIZE) * 2 + MARGIN * 3;
  int windowHeight = SNAKE_FIELD_H * CELL_SIZE + MARGIN * 2;
  setFixedSize(windowWidth, windowHeight);
  setWindowTitle("Snake");

  state_ = controller_.updateCurrentState();
  connect(&timer_, &QTimer::timeout, this, &SnakeWindow::updateGame);
  timer_.start(1);
}

SnakeWindow::~SnakeWindow() {}

void SnakeWindow::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);

  drawGameField(painter);
  drawInfoField(painter);
  drawText(painter);
}

void SnakeWindow::drawGameField(QPainter &painter) {
  painter.setPen(Qt::black);
  painter.setBrush(Qt::gray);
  painter.drawRect(MARGIN, MARGIN, CELL_SIZE * SNAKE_FIELD_W,
                   CELL_SIZE * SNAKE_FIELD_H);

  for (int y = 0; y < SNAKE_FIELD_H; y++) {
    for (int x = 0; x < SNAKE_FIELD_W; x++) {
      QRect cellRect(MARGIN + x * CELL_SIZE, MARGIN + y * CELL_SIZE, CELL_SIZE,
                     CELL_SIZE);

      switch (state_.field[y][x]) {
        case CELL_SNAKE_HEAD:
          painter.setBrush(Qt::darkGreen);
          painter.drawRect(cellRect);
          break;
        case CELL_SNAKE_BODY:
          painter.setBrush(Qt::green);
          painter.drawRect(cellRect);
          break;
        case CELL_APPLE:
          painter.setBrush(Qt::red);
          painter.drawRect(cellRect);
          break;
        default:
          painter.setBrush(Qt::gray);
          painter.drawRect(cellRect);
          break;
      }
      painter.setPen(Qt::lightGray);
      painter.setBrush(Qt::NoBrush);
      painter.drawRect(cellRect);
    }
  }
}

void SnakeWindow::drawInfoField(QPainter &painter) {
  int offset = CELL_SIZE * SNAKE_FIELD_W;
  painter.setPen(Qt::black);
  painter.setBrush(Qt::gray);
  painter.drawRect(offset + 20, 10, CELL_SIZE * SNAKE_FIELD_W,
                   CELL_SIZE * SNAKE_FIELD_H);

  QFont infoFont = painter.font();
  infoFont.setPointSize(16);
  painter.setFont(infoFont);
  painter.setPen(Qt::black);

  painter.drawText(offset + 50, 100, "SCORE:");
  painter.drawText(offset + 200, 100, QString::number(state_.score));

  painter.drawText(offset + 50, 150, "HIGH SCORE:");
  painter.drawText(offset + 200, 150, QString::number(state_.high_score));

  painter.drawText(offset + 50, 200, "LEVEL:");
  painter.drawText(offset + 200, 200, QString::number(state_.level));

  painter.drawText(offset + 50, 420, "Controls:   ↑ ↓ ← →");
  painter.drawText(offset + 50, 470, "Pause:   'p'");
  painter.drawText(offset + 50, 520, "Exit game:   'q'");
}

void SnakeWindow::drawText(QPainter &painter) {
  QFont font = painter.font();
  font.setPointSizeF(font.pointSizeF() * 1.5);
  painter.setFont(font);
  QPen pen(Qt::black, 8);
  painter.setPen(pen);

  switch (state_.pause) {
    case INITIAL:
      painter.drawText(105, 250, "Press 's'");
      painter.drawText(65, 290, "to start game");
      break;
    case PAUSED:
      painter.drawText(115, 270, "PAUSE");
      break;
    case WIN:
      painter.drawText(85, 270, "YOU WIN :)");
      painter.drawText(55, 330, "'s' - New Game");
      painter.drawText(60, 370, "'q' - Quit");
      break;
    case GAMEOVER:
      painter.drawText(70, 270, "GAME OVER");
      painter.drawText(55, 330, "'s' - New Game");
      painter.drawText(60, 370, "'q' - Quit");
      break;

    default:
      break;
  }
}

void SnakeWindow::keyPressEvent(QKeyEvent *event) {
  UserAction_t action = static_cast<UserAction_t>(-1);

  switch (event->key()) {
    case Qt::Key_S:
      action = Start;
      break;
    case Qt::Key_P:
      action = Pause;
      break;
    case Qt::Key_Left:
      action = Left;
      break;
    case Qt::Key_Right:
      action = Right;
      break;
    case Qt::Key_Up:
      action = Up;
      break;
    case Qt::Key_Down:
      action = Down;
      break;
    case Qt::Key_Space:
      action = Action;
      break;
    case Qt::Key_Q:
      action = Terminate;
      close();
      break;
    default:
      break;
  }

  controller_.userInput(action, true);
  state_ = controller_.updateCurrentState();
  update();
}

void SnakeWindow::keyReleaseEvent(QKeyEvent *event) {
  UserAction_t action = static_cast<UserAction_t>(-1);

  switch (event->key()) {
    case Qt::Key_Space:
      action = Action;
      break;
    default:
      break;
  }

  if (action != static_cast<UserAction_t>(-1)) {
    controller_.userInput(action, false);
    state_ = controller_.updateCurrentState();
    update();
  }
}

void SnakeWindow::updateGame() {
  state_ = controller_.updateCurrentState();

  update();
}

void SnakeWindow::closeEvent(QCloseEvent *event) {
  emit closed();
  event->accept();
}

}  // namespace s21