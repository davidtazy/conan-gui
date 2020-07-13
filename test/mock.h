#pragma once
#include <interfaces/iprocess.h>
#include <util.h>
#include <algorithm>

struct MockProcess : IProcess {
  Args _args;
  OutProcess _out{0, "out", "err"};
  std::string _path;
  struct Out {
    std::string data;
    Std stream;
  };
  std::vector<Out> _outs;
  OutputStream _callback;

  MockProcess(OutProcess out = {}) : IProcess{} { _out = out; }

  void set_executable(std::string path) override { _path = path; }

  int long_call(const Args& args, OutputStream callback_p) override {
    _args = args;
    _callback = callback_p;

    _outs.clear();
    // dump cmd line
    using namespace std::literals::string_literals;
    std::string cmdline =
        std::accumulate(std::begin(args), std::end(args), ""s,
                        [](std::string& acc, const std::string& arg) -> decltype(auto) {
                          acc += " ";
                          return acc += arg;
                        });
    callback_p(cmdline, Std::Cmd);

    for (auto stdout_line : util::split_line(_out.out)) {
      _outs.push_back({stdout_line, Std::Out});
    }

    for (auto stderr_line : util::split_line(_out.err)) {
      _outs.push_back({stderr_line, Std::Err});
    }

    _outs.push_back({std::to_string(_out.ret_code), Std::RetCode});

    return 0;
  }

  OutProcess call(const Args& args) override {
    _args = args;
    return _out;
  }

  std::string which() override { return _path; }

  void process_long_call_in_one_batch() {
    for (auto line : _outs) {
      _callback(line.data, line.stream);
    }
    _outs.clear();
  }
};
