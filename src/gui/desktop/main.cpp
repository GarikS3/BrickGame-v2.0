#include <QApplication>

#include "menu_window.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::MenuWindow menu;
  menu.show();
  return a.exec();
}
