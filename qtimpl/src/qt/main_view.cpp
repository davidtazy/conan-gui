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

  connect(ui->button_install, &QAbstractButton::clicked, this, &MainView::onInstallCmd);

  connect(ui->button_install_dir, &QAbstractButton::clicked, this, &MainView::onSelectInstallDir);
  connect(ui->button_path_or_reference, &QAbstractButton::clicked, this,
          &MainView::onSelectPathOrReference);
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
  ui->combo_profile->clear();
  for (auto pr : profiles) {
    ui->profile_combo->addItem(pr.c_str());
    ui->combo_profile->addItem(pr.c_str());
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

void MainView::setBuildPolicies(std::vector<std::string> policies) {
  ui->combo_build_policy->clear();
  QStringList policies_list;
  for (const auto& p : policies) {
    policies_list.push_back(p.c_str());
  }
  ui->combo_build_policy->addItems(policies_list);
}

void MainView::onInstallCmd() {
  assert(on_install_cmdline_callback);

  IConan::InstallCmdLine cmd;

  cmd.install_folder = ui->line_install_dir->text().trimmed().toStdString();
  cmd.path_or_reference = ui->line_path_or_reference->text().trimmed().toStdString();
  if (ui->check_build_policy->isChecked()) {
    cmd.policy = ui->combo_build_policy->currentText().toStdString();
  }

  if (ui->check_profile->isChecked()) {
    cmd.profile = ui->combo_profile->currentText().toStdString();
  }

  if (ui->check_extra_params->isChecked()) {
    auto params = ui->line_extra_params->text().split(" ", QString::SkipEmptyParts);
    std::vector<std::string> args;
    for (auto arg : params) {
      args.push_back(arg.toStdString());
    }
  }

  auto text_edit = this->ui->text_edit_install;

  on_install_cmdline_callback(cmd, [text_edit](std::string data, Std std_type) {
    Qt::GlobalColor colour = [std_type]() {
      switch (std_type) {
        case Std::Cmd:
          return Qt::darkGreen;
        case Std::Out:
          return Qt::darkBlue;
        case Std::Err:
          return Qt::darkRed;
        case Std::RetCode:
          return Qt::black;
      }
      throw std::runtime_error("unknow StdType");
    }();

    QTextCharFormat tf = text_edit->currentCharFormat();
    tf.setForeground(QBrush(colour));
    text_edit->setCurrentCharFormat(tf);

    if (std_type == Std::RetCode) {
      text_edit->appendPlainText("\nreturn code:");
    }
    text_edit->appendPlainText(data.c_str());
  });
}

void MainView::onSelectInstallDir() {
  auto dir = QFileDialog::getExistingDirectory(this, tr("select install folder"));
  if (dir.size()) {
    ui->line_install_dir->setText(dir);
  }
}

void MainView::onSelectPathOrReference() {
  auto dir = QFileDialog::getExistingDirectory(this, tr("select path"), "",
                                               QFileDialog::DontUseNativeDialog);
  if (dir.size()) {
    ui->line_path_or_reference->setText(dir);
  }
}

}  // namespace conanqt
