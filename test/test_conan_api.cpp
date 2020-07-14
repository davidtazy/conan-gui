#include <array>
#include <bitset>
#include <catch2/catch.hpp>

#include <conan.h>
#include <interfaces/iprocess.h>

#include "mock.h"

using namespace std::string_literals;

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
  auto out = util::join_path("home","user",".conan");


  auto mock = MockProcess{{0, out, ""}};
  Conan conan{&mock};

  REQUIRE(conan.profile_path() == util::join_path("home","user",".conan","profiles"));
  REQUIRE(conan.profile_path("default") == util::join_path("home","user",".conan","profiles","default"));
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

TEST_CASE("conan install with all parameters") {
  IConan::InstallCmdLine cmd;
  cmd.install_folder = "/a/b/c";
  cmd.path_or_reference = "/d/e/f";
  cmd.policy = "missing";
  cmd.profile = "clang10";
  cmd.extra_params = {"aaa", "bbb"};

  auto mock = MockProcess{{0, "", ""}};
  Conan conan{&mock};

  REQUIRE(conan.toArgs(cmd) == Args{"install", "/d/e/f", "--install-folder", "/a/b/c", "--build",
                                    "missing", "--profile", "clang10", "aaa", "bbb"});
}

TEST_CASE("conan install minimal parameters") {
  IConan::InstallCmdLine cmd;
  cmd.install_folder = "/a/b/c";
  cmd.path_or_reference = "/d/e/f";

  auto mock = MockProcess{{0, "", ""}};
  Conan conan{&mock};

  REQUIRE(conan.toArgs(cmd) == Args{"install", "/d/e/f", "--install-folder", "/a/b/c"});
}

TEST_CASE("conan install without path_or_reference throw an error") {
  IConan::InstallCmdLine cmd;
  cmd.install_folder = "/a/b/c";

  auto mock = MockProcess{{0, "", ""}};
  Conan conan{&mock};

  REQUIRE_THROWS_AS(conan.toArgs(cmd), std::runtime_error);
}

TEST_CASE("conan install without install folder throw an error") {
  IConan::InstallCmdLine cmd;
  cmd.path_or_reference = "/d/e/f";

  auto mock = MockProcess{{0, "", ""}};
  Conan conan{&mock};

  REQUIRE_THROWS_AS(conan.toArgs(cmd), std::runtime_error);
}

TEST_CASE("conan install dump stdout and stderr in real time") {
  IConan::InstallCmdLine cmd;
  cmd.install_folder = "/a/b/c";
  cmd.path_or_reference = "/d/e/f";

  auto mock = MockProcess{{0, "blabla\nblibli", "bobo\nbubu"}};
  Conan conan{&mock};

  struct Out {
    std::string data;
    Std stream;
    bool operator==(const Out& other) const { return data == other.data && stream == other.stream; }
  };
  std::vector<Out> outs;

  OutputStream stream = [&outs](const std::string& data, Std stream) {
    outs.push_back({data, stream});
  };

  conan.install(cmd, stream);
  REQUIRE(outs.size() == 1);
  REQUIRE(outs.front().stream == Std::Cmd);
  REQUIRE(outs.front().data == " install /d/e/f --install-folder /a/b/c");
  outs.clear();

  mock.process_long_call_in_one_batch();

  REQUIRE(outs.size() == 5);
  REQUIRE(outs == std::vector<Out>{{"blabla", Std::Out},
                                   {"blibli", Std::Out},
                                   {"bobo", Std::Err},
                                   {"bubu", Std::Err},
                                   {"0", Std::RetCode}});
}

// https://github.com/arun11299/cpp-subprocess
// http://templated-thoughts.blogspot.com/2016/03/sub-processing-with-modern-c.html
