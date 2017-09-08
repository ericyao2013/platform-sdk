#define CATCH_CONFIG_MAIN

#include <airmap/platform/interface.h>
#include <airmap/platform/standard_paths.h>

#include <airmap/platform/null/interface.h>

#include <catch/catch.hpp>

TEST_CASE("platform") {
  SECTION("enum platform::StandardPaths::Scope") {
    SECTION("is inserted correctly into output stream") {
      {
        std::stringstream ss;
        ss << airmap::platform::StandardPaths::Scope::system;
        REQUIRE(ss.str() == "system");
      }
      {
        std::stringstream ss;
        ss << airmap::platform::StandardPaths::Scope::user;
        REQUIRE(ss.str() == "user");
      }
    }
  }
  SECTION("enum platform::StandardPaths::Location") {
    SECTION("is inserted correctly into output stream") {
      {
        std::stringstream ss;
        ss << airmap::platform::StandardPaths::Location::cache;
        REQUIRE(ss.str() == "cache");
      }
      {
        std::stringstream ss;
        ss << airmap::platform::StandardPaths::Location::config;
        REQUIRE(ss.str() == "config");
      }
      {
        std::stringstream ss;
        ss << airmap::platform::StandardPaths::Location::data;
        REQUIRE(ss.str() == "data");
      }
      {
        std::stringstream ss;
        ss << airmap::platform::StandardPaths::Location::runtime;
        REQUIRE(ss.str() == "runtime");
      }
    }
  }
}

TEST_CASE("null platform") {
  SECTION("always returns initial path") {
    airmap::platform::null::Interface itf;
    for (auto scope : {airmap::platform::StandardPaths::Scope::user, airmap::platform::StandardPaths::Scope::system})
      for (auto location :
           {airmap::platform::StandardPaths::Location::cache, airmap::platform::StandardPaths::Location::config,
            airmap::platform::StandardPaths::Location::data, airmap::platform::StandardPaths::Location::runtime})
        REQUIRE(itf.standard_paths().path(scope, location) == airmap::platform::initial_path());
  }
}

#if defined(AIRMAP_PLATFORM_LINUX)

TEST_CASE("linux platform") {
  SECTION("never throws for queries for paths") {
    airmap::platform::null::Interface itf;
    for (auto scope : {airmap::platform::StandardPaths::Scope::user, airmap::platform::StandardPaths::Scope::system})
      for (auto location :
           {airmap::platform::StandardPaths::Location::cache, airmap::platform::StandardPaths::Location::config,
            airmap::platform::StandardPaths::Location::data, airmap::platform::StandardPaths::Location::runtime})
        REQUIRE_NOTHROW(itf.standard_paths().path(scope, location));
  }
}

#endif  // AIRMAP_PLATFORM_LINUX
