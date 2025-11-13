#include "tetris_window.h"

namespace s21 {

TetrisWindow::TetrisWindow(QWidget *parent) : QWidget(parent) {
  int windowWidth = (TETRIS_FIELD_W * CELL_SIZE) * 2 + MARGIN * 3;
  int windowHeight = TETRIS_FIELD_H * CELL_SIZE + MARGIN * 2;
  setFixedSize(windowWidth, windowHeight);
  setWindowTitle("Tetris");

  initStartState();
  state_ = updateCurrentState();

  connect(&timer_, &QTimer::timeout, this, &TetrisWindow::updateGame);
  timer_.start(1);
}

TetrisWindow::~TetrisWindow() { freeMemory(); }

void TetrisWindow::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);

  drawGameField(painter);
  drawInfoField(painter);
  drawText(painter);
}

void TetrisWindow::drawGameField(QPainter &painter) {
  painter.setPen(Qt::black);
  painter.setBrush(Qt::gray);
  painter.drawRect(MARGIN, MARGIN, CELL_SIZE * TETRIS_FIELD_W,
                   CELL_SIZE * TETRIS_FIELD_H);

  for (int y = 0; y < TETRIS_FIELD_H; y++) {
    for (int x = 0; x < TETRIS_FIELD_W; x++) {
      QRect cellRect(MARGIN + x * CELL_SIZE, MARGIN + y * CELL_SIZE, CELL_SIZE,
                     CELL_SIZE);

      if (state_.field[y][x]) {
        painter.setBrush(Qt::blue);
        painter.drawRect(cellRect);
      } else {
        painter.setBrush(Qt::gray);
        painter.drawRect(cellRect);
      }

      painter.setPen(Qt::lightGray);
      painter.setBrush(Qt::NoBrush);
      painter.drawRect(cellRect);
    }
  }
}

void TetrisWindow::drawInfoField(QPainter &painter) {
  int offset = CELL_SIZE * TETRIS_FIELD_W;
  painter.setPen(Qt::black);
  painter.setBrush(Qt::gray);
  painter.drawRect(offset + MARGIN * 2, MARGIN, CELL_SIZE * TETRIS_FIELD_W,
                   CELL_SIZE * TETRIS_FIELD_H);

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

  painter.drawText(offset + 50, 250, "NEXT:");
  if (state_.pause != GameStart) drawNextFigure(painter);

  painter.drawText(offset + 50, 450, "Controls:   ← → ↓");
  painter.drawText(offset + 50, 480, "Rotate:   Space");
  painter.drawText(offset + 50, 510, "Pause:   'p'");
  painter.drawText(offset + 50, 540, "Exit game:   'q'");
}

void TetrisWindow::drawNextFigure(QPainter &painter) {
  for (int y = 0; y < FIGURE_SIZE; y++) {
    for (int x = 0; x < FIGURE_SIZE; x++) {
      if (state_.next[y][x]) {
        int pos_x = (CELL_SIZE * TETRIS_FIELD_W) + 140 + x * CELL_SIZE;
        int pos_y = 230 + y * CELL_SIZE;

        QRect cellRect(pos_x, pos_y, CELL_SIZE, CELL_SIZE);
        painter.setBrush(Qt::blue);
        painter.drawRect(cellRect);
      }
    }
  }
}

void TetrisWindow::drawText(QPainter &painter) {
  QFont font = painter.font();
  font.setPointSizeF(font.pointSizeF() * 1.5);
  painter.setFont(font);
  QPen pen(Qt::black, 8);
  painter.setPen(pen);

  switch (state_.pause) {
    case GameStart:
      painter.drawText(105, 250, "Press 's'");
      painter.drawText(65, 290, "to start game");
      break;
    case GamePaused:
      painter.drawText(115, 270, "PAUSE");
      break;
    case GameFinished:
      painter.drawText(70, 270, "GAME OVER");
      painter.drawText(55, 330, "'s' - New Game");
      painter.drawText(60, 370, "'q' - Quit");
      break;
    default:
      break;
  }
}

void TetrisWindow::updateGame() {
  handleGameState();
  state_ = updateCurrentState();

  update();
}

void TetrisWindow::keyPressEvent(QKeyEvent *event) {
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

  userInput(action, false);
  update();
}

void TetrisWindow::closeEvent(QCloseEvent *event) {
  emit closed();
  event->accept();
}

}  // namespace s21