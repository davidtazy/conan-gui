#include "conan.h"

using namespace std::literals::string_literals;

Args Conan::toArgs(const IConan::InstallCmdLine& cmd) const {
  Args args{"install"};

  if (cmd.path_or_reference.empty()) {
    throw std::runtime_error("conan install : path_or_reference is empty");
  }

  if (cmd.install_folder.empty()) {
    throw std::runtime_error("conan install : install_folder is empty");
  }

  args.push_back(cmd.path_or_reference);

  args.push_back("--install-folder"s);
  args.push_back(cmd.install_folder);

  if (cmd.policy.size()) {
    args.push_back("--build"s);
    if (cmd.policy != "all") {
      args.push_back(cmd.policy);
    }
  }

  if (cmd.profile.size()) {
    args.push_back("--profile"s);
    args.push_back(cmd.profile);
  }

  for (auto extra_param : cmd.extra_params) {
    args.push_back(extra_param);
  }

  return args;
}

void Conan::install(InstallCmdLine cmd, OutputStream stream) {
  auto args = toArgs(cmd);

  _process->long_call(args, stream);
}
