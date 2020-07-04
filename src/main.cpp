#include <iostream>
#include <sstream>
#include "interfaces/iconan.h"
#include "qt/graphic_event_loop.h"
#include "qt/main_view.h"
#include "qt/process.h"

namespace util {
auto split_line(const std::string& chaine) {
  std::vector<std::string> ret;
  std::stringstream ss(chaine);
  std::string sousChaine;
  while (std::getline(ss, sousChaine)) {
    ret.push_back(sousChaine);
  }
  return ret;
}

std::string join_path(std::string p1, const std::string& p2) {
  char sep = '/';

#ifdef _WIN32
  sep = '\\';
#endif

  if (p1[p1.length()] != sep) {  // Need to add a
    p1 += sep;                   // path separator
  }
  return (p1 + p2);
}

}  // namespace util

struct Conan : public IConan {
  explicit Conan(IProcess* process) : _process{process} {}

  std::string version() const {
    auto [ret_code, sout, serr] = _process->call({"--version"});

    if (ret_code != 0) {
      throw std::runtime_error(serr);
    }

    return sout;
  }

  std::string which() const override { return _process->which(); }

  std::vector<std::string> profile_list() const override {
    auto [ret_code, sout, serr] = _process->call({"profile", "list"});

    if (ret_code != 0) {
      throw std::runtime_error(serr);
    }
    return util::split_line(sout);
  }

  std::string profile_path(std::string profile) const override {
    auto [ret_code, sout, serr] = _process->call({"config", "home"});

    if (ret_code != 0) {
      throw std::runtime_error(serr);
    }
    auto path = util::join_path(util::split_line(sout).at(0), "profiles");

    return util::join_path(path, profile);
  }

 private:
  IProcess* _process;
};

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
