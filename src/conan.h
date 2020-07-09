
#pragma once
#include <interfaces/iconan.h>
#include <interfaces/iprocess.h>
#include <sstream>
#include "util.h"

struct Conan : public IConan {
  explicit Conan(IProcess* process) : _process{process} {}

  std::string version() const {
    auto [ret_code, sout, serr] = _process->call({"--version"});

    if (ret_code != 0) {
      throw std::runtime_error(serr);
    }

    return sout;
  }

  std::vector<std::string> profile_list() const override {
    auto [ret_code, sout, serr] = _process->call({"profile", "list"});

    if (ret_code != 0) {
      throw std::runtime_error(serr);
    }
    return util::split_line(sout);
  }

  std::string home_path() const {
    auto [ret_code, sout, serr] = _process->call({"config", "home"});

    if (ret_code != 0) {
      throw std::runtime_error(serr);
    }
    return util::split_line(sout).at(0);
  }

  std::string profile_path(std::string profile = "") const override {
    auto home_path = this->home_path();

    auto path = util::join_path(home_path, "profiles");

    return util::join_path(path, profile);
  }

  std::vector<Remote> remote_list() const {
    auto [ret_code, sout, serr] = _process->call({"remote", "list"});
    if (ret_code != 0) {
      throw std::runtime_error(serr);
    }
    std::vector<Remote> remotes;
    for (auto line : util::split_line(sout)) {
      std::istringstream in(line);
      Remote remote;

      // extract name
      std::getline(in, remote.name, ':');
      in.ignore();

      // extract url
      in >> remote.url;

      // extract options
      std::string options;
      std::getline(in, options);
      remote.verify_ssl = options.find("Verify SSL: True") != std::string::npos;
      remote.enabled = options.find("Disabled: True") == std::string::npos;
      remotes.push_back(remote);
    }
    return remotes;
  }

  void remote_enable(std::string name, bool enable) {
    std::string cmd = (enable) ? "enable" : "disable";

    auto [ret_code, sout, serr] = _process->call({"remote", cmd, name});
    if (ret_code != 0) {
      throw std::runtime_error(serr);
    }
  }

  std::vector<std::string> build_policies() const override {
    return {"missing", "all", "outdated", "cascade"};
  }

  void install(InstallCmdLine cmd) override;
  Args toArgs(const IConan::InstallCmdLine& cmd) const;

 private:
  IProcess* _process;
};
