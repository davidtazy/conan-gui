#pragma once

#include <interfaces/iconan.h>
#include <interfaces/imainview.h>
#include <interfaces/iprocess.h>

class ConanGui {
  IProcess* process;
  IConan* conan;
  IMainView* view;

 public:
  ConanGui(IProcess* process, IConan* conan, IMainView* view)
      : process(process), conan(conan), view(view) {
    process->set_executable("conan");
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

      view->onSetConanExecutable([this](std::string conan_executable) {
        this->process->set_executable(conan_executable);
        this->Reset();
      });

      view->setBuildPolicies(conan->build_policies());
      view->onInstallCommand([this](IConan::InstallCmdLine cmd) {
        try {
          this->conan->install(cmd);
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
