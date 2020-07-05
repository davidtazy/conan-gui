#pragma once
#include <interfaces/iprocess.h>

struct MockProcess : IProcess {
  Args _args;
  OutProcess _out{0, "out", "err"};
  std::string _path;

  MockProcess(OutProcess out = {}) : IProcess{} { _out = out; }

  void set_executable(std::string path) override { _path = path; }

  int long_call(const Args& args) override {
    _args = args;
    return 0;
  }

  OutProcess call(const Args& args) override {
    _args = args;
    return _out;
  }

  std::string which() override { return _path; }
};
