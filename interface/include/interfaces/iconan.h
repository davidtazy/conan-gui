#pragma once
#include <string>
#include <vector>

struct IConan {
  virtual std::string version() const = 0;
  virtual std::string which() const = 0;
  virtual std::vector<std::string> profile_list() const = 0;

  virtual std::string profile_path(std::string) const = 0;
};