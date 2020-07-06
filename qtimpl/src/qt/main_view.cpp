#include "main_view.h"
#include <QDesktopServices>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QUrl>
#include <cassert>
#include "ui_main_view.h"  // for MainView

namespace conanqt {
MainView::MainView() : ui{new Ui::MainView} {
  ui->setupUi(this);

  connect(ui->profile_button, &QPushButton::clicked, [this]() {
    if (this->on_show_profile_callback) {
      this->on_show_profile_callback(this->ui->profile_combo->currentText().toStdString());
    }
  });

  connect(ui->path_button, &QAbstractButton::clicked, this, &MainView::onSelectConanExecutable);

  ui->remotes_table->setModel(&remote_model.model);

  show();
}

void MainView::clear() {
  ui->path_line->setText("");
  remote_model.model.clear();
  ui->profile_combo->clear();
  ui->conan_group->setTitle("");
}

void MainView::setVersion(std::string version) {
  ui->conan_group->setTitle(version.c_str());
}

void MainView::setPath(std::string path) {
  ui->path_line->setText(path.c_str());
}

void MainView::setProfiles(std::vector<std::string> profiles) {
  ui->profile_combo->clear();
  for (auto pr : profiles) {
    ui->profile_combo->addItem(pr.c_str());
  }

  QString tooltip;
  if (profiles.empty()) {
    tooltip = tr("you should create a profile");
  }
  ui->profile_combo->setToolTip(tooltip);
}

void MainView::showProfile(std::string profile_path) {
  QDesktopServices::openUrl(QUrl::fromLocalFile(profile_path.c_str()));
}

void MainView::popupError(std::string error) {
  QMessageBox::critical(this, tr("error"), error.c_str());
}

void MainView::setRemotes(const std::vector<IConan::Remote>& remotes) {
  remote_model.reset(remotes);

  // ensure all text in column are visible
  ui->remotes_table->setVisible(false);
  ui->remotes_table->resizeColumnsToContents();
  ui->remotes_table->setVisible(true);
}

void MainView::onSelectConanExecutable() {
  assert(on_set_conan_executable_callback);

  QMessageBox::StandardButton ret = QMessageBox::question(this, tr("select conan executable"),
                                                          tr("use conan executable in the PATH ?"));

  if (ret == QMessageBox::Yes) {
    on_set_conan_executable_callback("conan");
    return;
  }

  auto exe = QFileDialog::getOpenFileName(this, tr("select conan executable"));

  if (exe.size()) {
    on_set_conan_executable_callback(exe.toStdString());
  }
}

}  // namespace conanqt
