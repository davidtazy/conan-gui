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

  struct InstallCmdLine {
    std::string path_or_reference;
    std::string install_folder;
    std::string profile;
    std::string policy;
    std::vector<std::string> extra_params;
  };

  // build policies may be different through version
  virtual std::vector<std::string> build_policies() const = 0;

  virtual void install(InstallCmdLine cmd) = 0;
};