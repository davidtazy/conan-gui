#pragma once
#include <interfaces/iconan.h>
#include <QObject>
#include <QStandardItemModel>
#include <functional>

namespace conanqt {

class RemoteListModel : public QObject {
  Q_OBJECT

  std::function<void(std::string, bool)> enabled_callback;

 public:
  QStandardItemModel model;

  RemoteListModel();

  void reset(std::vector<IConan::Remote> remotes);

  void onItemEnabledChanged(std::function<void(std::string, bool)> callback) {
    enabled_callback = callback;
  }

 public slots:

  void onItemChanged(QStandardItem* item);
};

}  // namespace conanqt