#pragma once
#include <functional>
#include <string>
#include <vector>

enum class Std { Out, Err, Cmd, RetCode };
using OutputStream = std::function<void(const std::string& data, Std)>;

struct OutProcess {
  int ret_code;
  std::string out;
  std::string err;
};

using Args = std::vector<std::string>;

struct IProcess {
  virtual void set_executable(std::string path) = 0;

  virtual int long_call(const Args& args, OutputStream callback_p) = 0;

  virtual OutProcess call(const Args& args) = 0;

  virtual std::string which() = 0;  // on linux call which, on windows, call where
};
