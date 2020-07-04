
#include <qmainwindow.h>  // for QMainWindow
#include <qobject.h>      // for QObject
#include <qobjectdefs.h>  // for Q_OBJECT, slots
#include <qstring.h>      // for QString
#include "interfaces/imainview.h"

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

 private slots:
  void toto() {}

 private:
  Ui::MainView* ui{};
  std::function<void(std::string)> on_show_profile_callback;
};
}  // namespace conanqt