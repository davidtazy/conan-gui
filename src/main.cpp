#include <iostream>
#include <sstream>
#include "conan.h"
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

class ConanGui {
  IProcess* process;
  IConan* conan;
  IMainView* view;

 public:
  ConanGui(IProcess* process, IConan* conan, IMainView* view)
      : process(process), conan(conan), view(view) {}

  void Reset() {
    try {
      view->setVersion(conan->version());
      view->setPath(conan->which());
      view->setProfiles(conan->profile_list());

      view->onShowProfile([this](std::string profile) {
        auto profile_path = this->conan->profile_path(profile);
        this->view->showProfile(profile_path);
      });

      view->setRemotes(conan->remote_list());

      view->onEnableRemote([this](std::string name, bool enabled) {
        // apply cmd
        this->conan->remote_enable(name, enabled);

        // reload lit
        view->setRemotes(conan->remote_list());
      });
    } catch (std::runtime_error e) {
      view->popupError(e.what());
    }
  }
};

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
