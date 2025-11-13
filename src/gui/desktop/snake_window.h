#pragma once

#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>

#include "../../brick_game/snake/controller/controller.h"

namespace s21 {

class SnakeWindow : public QWidget {
  Q_OBJECT

 public:
  SnakeWindow(QWidget *parent = nullptr);
  ~SnakeWindow();

 signals:
  void closed();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
  void closeEvent(QCloseEvent *event) override;

 private:
  static constexpr int CELL_SIZE = 30;
  static constexpr int MARGIN = 10;

  void drawGameField(QPainter &painter);
  void drawInfoField(QPainter &painter);
  void drawText(QPainter &painter);
  void updateGame();

  SnakeController controller_;
  GameInfo_t state_;
  QTimer timer_;
};

}  // namespace s21