#include "menu_window.h"

#include "snake_window.h"
#include "tetris_window.h"

namespace s21 {

MenuWindow::MenuWindow(QWidget *parent) : QWidget(parent) { drawMenuWindow(); }

MenuWindow::~MenuWindow() {}

void MenuWindow::drawMenuWindow() {
  setFixedSize(500, 500);

  QVBoxLayout *menuLayout = new QVBoxLayout(this);

  QLabel *titleLabel = new QLabel("BrickGame Menu", this);
  titleLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

  QFont titleFont = titleLabel->font();
  titleFont.setPointSize(20);
  titleFont.setBold(true);
  titleLabel->setFont(titleFont);

  tetrisButton = new QPushButton("Tetris", this);
  tetrisButton->setFixedSize(240, 50);
  snakeButton = new QPushButton("Snake", this);
  snakeButton->setFixedSize(240, 50);
  exitButton = new QPushButton("Exit", this);
  exitButton->setFixedSize(240, 50);

  menuLayout->addWidget(titleLabel);
  menuLayout->addStretch();
  menuLayout->addWidget(tetrisButton);
  menuLayout->addWidget(snakeButton);
  menuLayout->addWidget(exitButton);
  menuLayout->addStretch();

  menuLayout->setContentsMargins(130, 50, 130, 50);
  menuLayout->setAlignment(Qt::AlignHCenter);
  menuLayout->setSpacing(30);

  connect(tetrisButton, &QPushButton::clicked, this,
          &MenuWindow::startTetrisGame);
  connect(snakeButton, &QPushButton::clicked, this,
          &MenuWindow::startSnakeGame);
  connect(exitButton, &QPushButton::clicked, this, &QApplication::quit);
}

void MenuWindow::startTetrisGame() {
  TetrisWindow *tetrisWindow = new TetrisWindow();
  tetrisWindow->show();
  this->hide();

  connect(tetrisWindow, &TetrisWindow::closed, this, &QWidget::show);
}

void MenuWindow::startSnakeGame() {
  SnakeWindow *snakeWindow = new SnakeWindow();
  snakeWindow->show();
  this->hide();

  connect(snakeWindow, &SnakeWindow::closed, this, &QWidget::show);
}

}  // namespace s21