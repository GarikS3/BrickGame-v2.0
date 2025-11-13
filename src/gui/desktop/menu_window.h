#pragma once

#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

namespace s21 {

class MenuWindow : public QWidget {
  Q_OBJECT

 public:
  MenuWindow(QWidget *parent = nullptr);
  ~MenuWindow();

 private:
  void drawMenuWindow();

  QLabel *titleLabel;
  QPushButton *snakeButton;
  QPushButton *tetrisButton;
  QPushButton *exitButton;

 private slots:
  void startSnakeGame();
  void startTetrisGame();
};

}  // namespace s21