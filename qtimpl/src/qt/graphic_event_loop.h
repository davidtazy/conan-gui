#pragma once

#include <QApplication>
#include <QFile>
#include <QFontDatabase>

struct IGraphicsEventLoop {
  virtual int execute() = 0;
};

namespace conanqt {
struct GraphicEventLoop : IGraphicsEventLoop {
  GraphicEventLoop(int argc, char** argv);
  GraphicEventLoop(QApplication& app);

  int execute() override;

  QApplication& app;
};

}  // namespace conanqt