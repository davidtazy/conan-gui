
#include "graphic_event_loop.h"

static void init_resource() {
  // cannot be called from nested namespace
  Q_INIT_RESOURCE(resource);
}

namespace conanqt {
// GraphicEventLoop::GraphicEventLoop(int argc, char** argv) : app(argc, argv) {
GraphicEventLoop::GraphicEventLoop(QApplication& app) : app(app) {
  app.setApplicationDisplayName("conan-gui");
  app.setApplicationName("conan-gui");
  app.setApplicationVersion("0.0.0");
  app.setOrganizationDomain("davidtazy");
  app.setOrganizationName("davidtazy");

  init_resource();
  QFontDatabase::addApplicationFont(":/DejaVuSans.ttf");

  app.setFont(QFont("dejavu", 12, QFont::Normal, false));

  app.processEvents();
}

int GraphicEventLoop::execute() {
  return app.exec();
}

}  // namespace conanqt