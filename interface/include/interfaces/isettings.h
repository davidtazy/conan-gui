#pragma once
#include <string>

struct ISettings {
  virtual void write(std::string key, std::string value) = 0;
  virtual std::string read(std::string key, std::string default_value) const = 0;

  // YAGNI
  // virtual int read(std::string key, int default_value) const = 0;
  // virtual bool read(std::string key, bool default_value) const = 0;
  // virtual bool has(std::string key) const = 0;
};