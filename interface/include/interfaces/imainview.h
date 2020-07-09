#pragma once
#include <functional>
#include <string>
#include <vector>
#include "interfaces/iconan.h"

struct IMainView {
  virtual void setVersion(std::string version) = 0;
  virtual void setPath(std::string path) = 0;
  virtual void setProfiles(std::vector<std::string> profiles) = 0;

  virtual void onShowProfile(std::function<void(std::string)> callback) = 0;
  virtual void showProfile(std::string profile_path) = 0;

  virtual void popupError(std::string error) = 0;

  virtual void setRemotes(const std::vector<IConan::Remote>& remotes) = 0;
  virtual void onEnableRemote(std::function<void(std::string, bool)> callback) = 0;

  virtual void onSetConanExecutable(std::function<void(std::string)>) = 0;

  virtual void clear() = 0;

  virtual void setBuildPolicies(std::vector<std::string> policies) = 0;

  virtual void onInstallCommand(std::function<void(IConan::InstallCmdLine)> callback) = 0;
};