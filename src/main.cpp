#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <iostream>
#include "qt/main_view.h"

int main(int argc, char** argv) {
  std::cout << "hello conan-gui !!! " << std::endl;

  QApplication a(argc, argv);

  Q_INIT_RESOURCE(resource);
  std::cout << "ttf " << QFile::exists(":/DejaVuSans.ttf") << std::endl;
  QFontDatabase::addApplicationFont(":/DejaVuSans.ttf");

  a.setFont(QFont("dejavu", 12, QFont::Normal, false));

  MainView view;
  view.show();

  return a.exec();
}