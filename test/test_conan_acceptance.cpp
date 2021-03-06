
#include <catch2/catch.hpp>

#include <conangui.h>
#include <util.h>
#include "mock.h"

struct FakeConan : public IConan {
  bool throw_error_on_version_call{false};
  std::string str_version = "Conan version 1.25.2";
  std::vector<std::string> vect_profiles = {"default", "gcc10"};
  std::vector<InstallCmdLine> install_cmd;

  const bool enabled = true, disabled = false, ssl = true, nossl = false;
  std::vector<IConan::Remote> _remotes = {
      {enabled, "conan-center", "http://conan-center.com", ssl},
      {disabled, "bincrafters", "http://bincrafters.com", nossl}};

  std::string version() const override {
    if (throw_error_on_version_call) {
      throw std::runtime_error("error");
    }
    return str_version;
  };

  std::vector<std::string> profile_list() const override { return vect_profiles; };

  std::string profile_path(std::string profile = "") const override {
    return util::join_path("home",".conan","profiles", profile);
  };

  std::vector<Remote> remote_list() const override { return _remotes; };
  void remote_enable(std::string name, bool enable) {
    for (auto& remote : _remotes) {
      if (remote.name == name) {
        remote.enabled = enable;
      }
    }
  };

  std::vector<std::string> build_policies() const override {
    return {"missing", "all", "outdated", "cascade"};
  }
  void install(InstallCmdLine cmd, OutputStream stream) override { install_cmd.push_back(cmd); }
};

struct FakeView : public IMainView {
  std::string _version{};
  std::string _path{};
  std::vector<std::string> _profiles{};
  std::function<void(std::string)> _callback_on_show_profile;
  std::function<void(std::string)> _callback_set_conan_executable;

  std::vector<std::string> _show_profile_triggers;
  std::vector<std::string> _popup_error_triggers;
  int _clear_view_counter{};

  std::vector<IConan::Remote> _remotes{};
  std::function<void(std::string name, bool enable)> _callback_remote_enable;

  std::function<void(IConan::InstallCmdLine, OutputStream)> _install_cmdline_callback;

  std::vector<std::string> _policies;

  FakeView() = default;

  void setVersion(std::string version) override { _version = version; };
  void setPath(std::string path) override { _path = path; };
  void setProfiles(std::vector<std::string> profiles) override { _profiles = profiles; };

  void onShowProfile(std::function<void(std::string)> callback) override {
    _callback_on_show_profile = callback;
  };
  void showProfile(std::string profile_path) override {
    _show_profile_triggers.push_back(profile_path);
  };

  void popupError(std::string error) override { _popup_error_triggers.push_back(error); };

  void setRemotes(const std::vector<IConan::Remote>& remotes) override { _remotes = remotes; };
  void onEnableRemote(std::function<void(std::string name, bool enable)> callback) override {
    _callback_remote_enable = callback;
  };

  void onSetConanExecutable(std::function<void(std::string)> callback) override {
    _callback_set_conan_executable = callback;
  }

  void clear() override {
    _version = "";
    _path = "";
    _profiles.clear();
    _remotes.clear();
    _clear_view_counter++;

    // _show_profile_triggers.clear();
    //_popup_error_triggers.clear();
  }

  void onInstallCommand(
      std::function<void(IConan::InstallCmdLine, OutputStream)> callback) override {
    _install_cmdline_callback = callback;
  }

  void setBuildPolicies(std::vector<std::string> policies) override { _policies = policies; }
};
#include <map>
struct FakeSettings:public ISettings{
  std::map<std::string,std::string> dico;

 void write(std::string key, std::string value)override{
   dico[key] = value;
 }
   std::string read(std::string key, std::string default_value) const override{
     auto it = dico.find(key);
     if( it != dico.end()){
       return it->second;
     }else{
       return default_value;
     }
   }
};

struct Builder {
  MockProcess process;
  FakeConan conan;
  FakeView view;
  FakeSettings settings;

  ConanGui gui;
  
  Builder() : gui{&process, &conan, &view, &settings} {}
};

TEST_CASE("conangui try to use conan in path by default") {
  Builder b;

  REQUIRE(b.process._path == "conan");
}

TEST_CASE(" popup error and view cleared if conan app cannot be started") {
  Builder b;

  REQUIRE(b.view._clear_view_counter == 0);
  b.conan.throw_error_on_version_call = true;

  b.gui.Reset();

  REQUIRE(b.view._popup_error_triggers.size() == 1);
  REQUIRE(b.view._clear_view_counter == 1);
}

TEST_CASE("display conan version ") {
  Builder b;

  b.gui.Reset();

  REQUIRE(b.view._version == b.conan.str_version);
}

TEST_CASE("user can select conan application path") {
  Builder b;

  b.gui.Reset();

  b.view._callback_set_conan_executable("other_conan");

  REQUIRE(b.view._path == "other_conan");
}

TEST_CASE("user selected conan application path is saved ") {
  Builder b;

  b.gui.Reset();

  b.view._callback_set_conan_executable("other_conan");

  REQUIRE(b.settings.read("conan_executable","_") == "other_conan");
}

TEST_CASE("display available profiles") {
  Builder b;

  b.gui.Reset();

  REQUIRE(b.view._profiles == b.conan.vect_profiles);
}

TEST_CASE("user can open selected profile") {
  Builder b;
  b.gui.Reset();

  // simulate user action
  b.view._callback_on_show_profile("default");

  REQUIRE(b.view._show_profile_triggers.size() == 1);
  REQUIRE(b.view._show_profile_triggers.front() == util::join_path("home",".conan","profiles","default"));
}

TEST_CASE("if no profile defined, user can open profiles dir") {
  Builder b;
  b.gui.Reset();

  // simulate user action
  b.view._callback_on_show_profile("");

  REQUIRE(b.view._show_profile_triggers.size() == 1);
  REQUIRE(b.view._show_profile_triggers.front() == util::join_path("home",".conan","profiles"));
}

TEST_CASE("display remotes status") {
  Builder b;
  b.gui.Reset();

  REQUIRE(b.view._remotes == b.conan._remotes);
}

TEST_CASE("user can enable or disable remote") {
  Builder b;
  b.gui.Reset();

  REQUIRE(b.view._remotes[0].enabled == true);

  b.view._callback_remote_enable("conan-center", b.conan.disabled);
  REQUIRE(b.view._remotes[0].enabled == false);

  b.view._callback_remote_enable("conan-center", b.conan.enabled);
  REQUIRE(b.view._remotes[0].enabled == true);
}

TEST_CASE("build policies are provided by conan and accesible to users") {
  Builder b;
  REQUIRE(b.view._policies.empty());

  b.gui.Reset();

  REQUIRE(b.view._policies.size() > 0);
}

TEST_CASE("user can call conan install command") {
  Builder b;
  b.gui.Reset();
  IConan::InstallCmdLine cmd;
  OutputStream stream;
  b.view._install_cmdline_callback(cmd, stream);

  REQUIRE(b.conan.install_cmd.size() == 1);
}
