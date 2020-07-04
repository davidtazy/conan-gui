#pragma once
#include <functional>
#include <string>

enum class Std { Out, Err, Cmd };
using OutputStream = std::function<void(const std::string& data, Std)>;

struct OutProcess {
  int ret_code;
  std::string out;
  std::string err;
};

using Args = std::vector<std::string>;

struct IProcess {
  virtual int long_call(const Args& args) = 0;

  virtual OutProcess call(const Args& args) = 0;

  virtual std::string which() = 0;  // on linux call which, on windows, call where
};