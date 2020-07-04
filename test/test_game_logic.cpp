#include <array>
#include <bitset>
#include <catch2/catch.hpp>

#include <interfaces/iprocess.h>

struct Conan {
  explicit Conan(std::unique_ptr<IProcess> process) : _process{std::move(process)} {}

  std::string version() const {
    auto [ret_code, sout, serr] = _process->call({"--version"});

    if (ret_code != 0) {
      throw std::runtime_error(serr);
    }

    return sout;
  }

 private:
  std::unique_ptr<IProcess> _process;
};

struct StubProcess : IProcess {
  StubProcess(std::string path, OutputStream output_callback = nullptr)
      : IProcess{} {
    _path = path;
  }

  int long_call(const Args& args) override {
    _args = args;
    return 0;
  }

  OutProcess call(const Args& args) override {
    _args = args;
    return {0, "out", "err"};
  }

  Args _args;
  std::string _path;

  std::string which() override { return _path; }
};

TEST_CASE("conan api") {
  auto process = std::make_unique<StubProcess>("");
  auto stub = process.get();
  Conan conan(std::move(process));

  SECTION("read version") {
    REQUIRE(conan.version() == "out");
    REQUIRE(stub->_args == Args{"--version"});
  }
}
// https://github.com/arun11299/cpp-subprocess
// http://templated-thoughts.blogspot.com/2016/03/sub-processing-with-modern-c.html
