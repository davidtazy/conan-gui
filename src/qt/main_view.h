
#include <qmainwindow.h>  // for QMainWindow
#include <qobject.h>      // for QObject
#include <qobjectdefs.h>  // for Q_OBJECT, slots
#include <qstring.h>      // for QString

namespace Ui {
class MainView;
}

class QMainWindow;

class MainView : public QMainWindow {
  Q_OBJECT
 public:
  MainView();

 private slots:
  void toto() {}

 private:
  Ui::MainView* ui{};
};
