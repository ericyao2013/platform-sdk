#define CATCH_CONFIG_MAIN

#include <helper.h>

#include <airmap/client.h>

#include <catch/catch.hpp>

#include <fstream>

TEST_CASE("airmap::Client") {
  SECTION("Configuration") {
    SECTION("can be parsed from valid json") {
      std::ifstream json{test::source_dir() + "/airmapd.valid.config.json"};
      auto config = airmap::Client::load_configuration_from_json(json);
      REQUIRE(config.host == "api.airmap.com");
      REQUIRE(config.version == airmap::Client::Version::production);
      REQUIRE(config.sso.host == "sso.airmap.io");
      REQUIRE(config.sso.port == 443);
      REQUIRE(config.telemetry.host == "api-udp-telemetry.airmap.com");
      REQUIRE(config.telemetry.port == 16060);
      REQUIRE(config.traffic.host == "mqtt-prod.airmap.io");
      REQUIRE(config.traffic.port == 8883);
      REQUIRE(config.credentials.api_key == "lalelu");
    }
  }
}
