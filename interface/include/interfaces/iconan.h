#pragma once
#include <string>
#include <tuple>
#include <vector>
struct IConan {
  virtual std::string version() const = 0;
  virtual std::vector<std::string> profile_list() const = 0;

  virtual std::string profile_path(std::string = "") const = 0;

  struct Remote {
    bool enabled;
    std::string name;
    std::string url;
    bool verify_ssl;
    bool operator==(const Remote& other) const {
      return std::make_tuple(enabled, name, url, verify_ssl) ==
             std::make_tuple(other.enabled, other.name, other.url, other.verify_ssl);
    }
  };
  virtual std::vector<Remote> remote_list() const = 0;
  virtual void remote_enable(std::string name, bool enable) = 0;
};