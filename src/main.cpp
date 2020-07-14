#include <iostream>
#include <sstream>
#include "conan.h"
#include "conangui.h"
#include "interfaces/iconan.h"
#include "qt/graphic_event_loop.h"
#include "qt/main_view.h"
#include "qt/process.h"
#include "qt/settings.h"




int main(int argc, char** argv) {
  std::cout << "hello conan-gui !!! " << std::endl;

  QApplication app{argc, argv};

  conanqt::GraphicEventLoop loop{app};
  conanqt::Settings settings;
  conanqt::Process process(settings.read("conan_executable","conan"));
  conanqt::MainView view;

  Conan conan(&process);

  ConanGui gui(&process, &conan, &view,&settings);
  gui.Reset();

  // view.show();

  // framework
  return loop.execute();
}
