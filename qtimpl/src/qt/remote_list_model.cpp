#include "remote_list_model.h"
#include <QDebug>
namespace conanqt {

enum ItemData { Enabled = Qt::UserRole + 1 };

RemoteListModel::RemoteListModel() {
  reset({});
  connect(&model, &QStandardItemModel::itemChanged, this, &RemoteListModel::onItemChanged);
}

QStandardItem* create_boolean_item(bool checked) {
  auto bool_item = new QStandardItem{""};
  bool_item->setCheckable(true);
  bool_item->setCheckState((checked) ? Qt::Checked : Qt::Unchecked);
  return bool_item;
}

QList<QStandardItem*> create_row(const IConan::Remote& remote) {
  auto enabled = create_boolean_item(remote.enabled);

  QString name = QString::fromStdString(remote.name);

  enabled->setData(QVariant{name}, static_cast<int>(ItemData::Enabled));

  auto name_item = new QStandardItem(name);

  auto url = new QStandardItem(QString::fromStdString(remote.url));
  auto ssl = create_boolean_item(remote.verify_ssl);

  for (auto item : {name_item, url, ssl}) {
    item->setEditable(false);
  }
  ssl->setCheckable(false);
  ssl->setToolTip("ssl is read only");

  return QList<QStandardItem*>{enabled, name_item, url, ssl};
}

void RemoteListModel::reset(std::vector<IConan::Remote> remotes) {
  model.clear();
  model.setHorizontalHeaderLabels({"enabled", "name", "url", "verify SSL"});

  int index = 0;
  for (const auto& remote : remotes) {
    model.appendRow(create_row(remote));
    index++;
  }
}

void RemoteListModel::onItemChanged(QStandardItem* item) {
  
  if (QVariant v = item->data(static_cast<int>(ItemData::Enabled)); v.type() == QVariant::String) {
    std::string name = v.toString().toStdString();
    bool enabled = (item->checkState() == Qt::Checked);
    qDebug() << "item " << name.c_str() << " enabled= " << enabled;

    if (enabled_callback)
      enabled_callback(name, enabled);
  }
}

}  // namespace conanqt