#pragma once

#include <interfaces/iconan.h>
#include <interfaces/imainview.h>
#include <interfaces/iprocess.h>
#include <interfaces/isettings.h>

class ConanGui {
  IProcess* process;
  IConan* conan;
  IMainView* view;
  ISettings* settings;

 public:
  ConanGui(IProcess* process, IConan* conan, IMainView* view, ISettings* settings)
      : process(process), conan(conan), view(view),settings(settings) {
    process->set_executable(settings->read("conan_executable","conan"));

    view->onSetConanExecutable([this](std::string conan_executable) {
        this->process->set_executable(conan_executable);
        this->settings->write("conan_executable",conan_executable);
        this->Reset();
      });
  }

  void Reset() {
    try {
      view->setVersion(conan->version());
      view->setPath(process->which());
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

      

      view->setBuildPolicies(conan->build_policies());
      view->onInstallCommand([this](IConan::InstallCmdLine cmd, OutputStream callback) {
        try {
          this->conan->install(cmd, callback);
        } catch (std::runtime_error e) {
          this->view->popupError(e.what());
        }
      });

    } catch (std::runtime_error e) {
      view->clear();
      view->popupError(e.what());
    }
  }
};
