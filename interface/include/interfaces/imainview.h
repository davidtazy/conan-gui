#pragma once
#include <functional>
#include <string>

struct IMainView {
  virtual void setVersion(std::string version) = 0;
  virtual void setPath(std::string path) = 0;
  virtual void setProfiles(std::vector<std::string> profiles) = 0;

  virtual void onShowProfile(std::function<void(std::string)> callback) = 0;
  virtual void showProfile(std::string profile_path) = 0;

  virtual void popupError(std::string error) = 0;
};