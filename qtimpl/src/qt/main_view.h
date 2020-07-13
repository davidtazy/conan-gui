
#include <qmainwindow.h>  // for QMainWindow
#include <qobject.h>      // for QObject
#include <qobjectdefs.h>  // for Q_OBJECT, slots
#include <qstring.h>      // for QString
#include "interfaces/imainview.h"
#include "qt/remote_list_model.h"

namespace Ui {
class MainView;
}
class QMainWindow;

namespace conanqt {
class MainView : public QMainWindow, public IMainView {
  Q_OBJECT
 public:
  MainView();

  void setVersion(std::string version) override;
  void setPath(std::string path) override;
  void setProfiles(std::vector<std::string> profiles) override;

  void onShowProfile(std::function<void(std::string)> callback) override {
    on_show_profile_callback = callback;
  }

  void showProfile(std::string profile_path) override;

  void popupError(std::string error) override;

  void setRemotes(const std::vector<IConan::Remote>& remotes) override;
  void onEnableRemote(std::function<void(std::string, bool)> callback) override {
    remote_model.onItemEnabledChanged(callback);
  }

  void onSetConanExecutable(std::function<void(std::string)> callback) override {
    on_set_conan_executable_callback = callback;
  }

  void clear() override;

  void onInstallCommand(
      std::function<void(IConan::InstallCmdLine, OutputStream)> callback) override {
    on_install_cmdline_callback = callback;
  }

  void setBuildPolicies(std::vector<std::string> policies) override;

 private slots:
  void onSelectConanExecutable();
  void onInstallCmd();
  void onSelectInstallDir();
  void onSelectPathOrReference();

 private:
  Ui::MainView* ui{};
  std::function<void(std::string)> on_show_profile_callback;
  RemoteListModel remote_model;
  std::function<void(std::string)> on_set_conan_executable_callback;
  std::function<void(IConan::InstallCmdLine, OutputStream)> on_install_cmdline_callback;
};
}  // namespace conanqt