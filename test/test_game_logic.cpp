#include <array>
#include <bitset>
#include <catch2/catch.hpp>
#include <functional>

struct UseCase {
  std::string name;
  std::string icon;
  std::string description;
  std::string url;
};

struct UseCases {
  void Register(const UseCase& use_case);

 private:
};

struct Profile {
  UseCase get_use_case() const { return {}; }
};

TEST_CASE("main view presents uses cases") {}

TEST_CASE("UseCases") {
  UseCase use_case{"profile", ":/profile.png", "bla bla bla", "https://conan-doc"};
  SECTION("has name") { REQUIRE(use_case.name == "profile"); }
  SECTION("has icon") { REQUIRE(use_case.icon == ":/profile.png"); }
  SECTION("has description") { REQUIRE(use_case.description == "bla bla bla"); }
  SECTION("has url") { REQUIRE(use_case.url == "https://conan-doc"); }
}