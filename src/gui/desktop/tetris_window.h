#pragma once

#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>

#define START_BUTTON 115
#define PAUSE_BUTTON 112
#define ACTION_BUTTON 32
#define TERMINATE_BUTTON 113

#ifdef __cplusplus
extern "C" {
#endif
#include "../../brick_game/tetris/backend.h"
#ifdef __cplusplus
}
#endif

namespace s21 {

class TetrisWindow : public QWidget {
  Q_OBJECT

 public:
  TetrisWindow(QWidget *parent = nullptr);
  ~TetrisWindow();

 signals:
  void closed();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void closeEvent(QCloseEvent *event) override;

 private:
  static constexpr int CELL_SIZE = 30;
  static constexpr int MARGIN = 10;

  void drawGameField(QPainter &painter);
  void drawInfoField(QPainter &painter);
  void drawNextFigure(QPainter &painter);
  void drawText(QPainter &painter);
  void updateGame();

  GameInfo_t state_;
  QTimer timer_;
};

}  // namespace s21