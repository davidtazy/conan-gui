#pragma once
#include <string>
#include <vector>

struct IConan {
  virtual std::string version() const = 0;
  virtual std::string which() const = 0;
  virtual std::vector<std::string> profile_list() const = 0;

  virtual std::string profile_path(std::string = "") const = 0;

  struct Remote {
    bool enabled;
    std::string name;
    std::string url;
    bool verify_ssl;
  };
  virtual std::vector<Remote> remote_list() const = 0;
  virtual void remote_enable(std::string name, bool enable) = 0;
};