#pragma once

#include <interfaces/iprocess.h>

namespace conanqt {

struct Process : public IProcess {
  Process(std::string path) : _path{path} {}

  int long_call(const Args& args) override;

  OutProcess call(const Args& args) override;

  std::string which() override;

 private:
  std::string _path;
};

}  // namespace conanqt
