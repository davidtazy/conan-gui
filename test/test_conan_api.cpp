#include <array>
#include <bitset>
#include <catch2/catch.hpp>

#include <conan.h>
#include <interfaces/iprocess.h>

using namespace std::string_literals;

struct MockProcess : IProcess {
  Args _args;
  OutProcess _out{0, "out", "err"};
  std::string _path;

  MockProcess(OutProcess out = {}) : IProcess{} { _out = out; }

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

TEST_CASE("conan --version") {
  auto mock = MockProcess{{0, "Conan version 1.25.2", ""}};

  Conan conan(&mock);

  REQUIRE(conan.version() == "Conan version 1.25.2");
  REQUIRE(mock._args == Args{"--version"});
}

TEST_CASE(" conan profile list") {
  auto out = R"_(clang10
default
gcc9
)_";

  auto mock = MockProcess{{0, out, ""}};
  Conan conan{&mock};

  REQUIRE(conan.profile_list() == std::vector<std::string>{"clang10", "default", "gcc9"});
  REQUIRE(mock._args == Args{"profile", "list"});
}

TEST_CASE(" conan profile path") {
  auto out = R"_(/home/user/.conan
)_";

  auto mock = MockProcess{{0, out, ""}};
  Conan conan{&mock};

  REQUIRE(conan.profile_path() == "/home/user/.conan/profiles/");
  REQUIRE(conan.profile_path("default") == "/home/user/.conan/profiles/default");
  REQUIRE(mock._args == Args{"config", "home"});
}

TEST_CASE("conan remote list") {
  auto out = R"_(conan-center: https://conan.bintray.com [Verify SSL: True]
pix4d: https://api.bintray.com/conan/pix4d/conan [Verify SSL: True, Disabled: True]
bincrafters: https://api.bintray.com/conan/bincrafters/public-conan [Verify SSL: False]
)_";

  auto mock = MockProcess{{0, out, ""}};
  Conan conan{&mock};

  auto remotes = conan.remote_list();

  REQUIRE(mock._args == Args{"remote", "list"});

  REQUIRE(remotes.size() == 3);

  REQUIRE(remotes[0].name == "conan-center");
  REQUIRE(remotes[1].name == "pix4d");
  REQUIRE(remotes[2].name == "bincrafters");

  REQUIRE(remotes[0].url == "https://conan.bintray.com");
  REQUIRE(remotes[1].url == "https://api.bintray.com/conan/pix4d/conan");
  REQUIRE(remotes[2].url == "https://api.bintray.com/conan/bincrafters/public-conan");

  REQUIRE(remotes[0].verify_ssl == true);
  REQUIRE(remotes[1].verify_ssl == true);
  REQUIRE(remotes[2].verify_ssl == false);

  REQUIRE(remotes[0].enabled == true);
  REQUIRE(remotes[1].enabled == false);
  REQUIRE(remotes[2].enabled == true);
}

TEST_CASE(" enable/disable remote") {
  auto mock = MockProcess{{0, "", ""}};
  Conan conan{&mock};

  auto remote_name = "conan-center"s;

  conan.remote_enable(remote_name, true);
  REQUIRE(mock._args == Args{"remote", "enable", remote_name});

  conan.remote_enable(remote_name, false);
  REQUIRE(mock._args == Args{"remote", "disable", remote_name});
}

// https://github.com/arun11299/cpp-subprocess
// http://templated-thoughts.blogspot.com/2016/03/sub-processing-with-modern-c.html
