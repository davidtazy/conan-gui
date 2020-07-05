#include <iostream>
#include <sstream>
#include "conan.h"
#include "conangui.h"
#include "interfaces/iconan.h"
#include "qt/graphic_event_loop.h"
#include "qt/main_view.h"
#include "qt/process.h"
struct ISettings {
  virtual void write(std::string key, std::string value) = 0;
  virtual std::string read(std::string key, std::string default_value) const = 0;

  // YAGNI
  // virtual int read(std::string key, int default_value) const = 0;
  // virtual bool read(std::string key, bool default_value) const = 0;
  // virtual bool has(std::string key) const = 0;
};

#include <QSettings>
namespace conanqt {
struct Settings : public ISettings {
  Settings() = default;

  void write(std::string key, std::string value) override {
    settings.setValue(key.c_str(), QString::fromStdString(value));
    settings.sync();
  };
  std::string read(std::string key, std::string default_value) const override {
    return settings.value(key.c_str(), QString::fromStdString(default_value))
        .toString()
        .toStdString();
  };

 private:
  QSettings settings;
};
}  // namespace conanqt

int main(int argc, char** argv) {
  std::cout << "hello conan-gui !!! " << std::endl;

  QApplication app{argc, argv};

  conanqt::GraphicEventLoop loop{app};
  conanqt::Process process("conan");
  conanqt::MainView view;

  Conan conan(&process);

  ConanGui gui(&process, &conan, &view);
  gui.Reset();

  // view.show();

  // framework
  return loop.execute();
}
