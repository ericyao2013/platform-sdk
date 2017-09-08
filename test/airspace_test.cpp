#include <airmap/airspace.h>

#include <catch/catch.hpp>

TEST_CASE("Airspace") {
  SECTION("default ctor yields an invalid airspace") {
    airmap::Airspace airspace;
    REQUIRE(airspace.type() == airmap::Airspace::Type::invalid);
  }
  SECTION("copy ctor yields instances that compare equal") {
    airmap::Airspace a1, a2;
    REQUIRE(a1 == a2);
  }
  SECTION("assignment operator yields instances that compare equal") {
    SECTION("for default ctor") {
      airmap::Airspace a1;
      auto a2 = a1;
      REQUIRE(a1 == a2);
    }
    SECTION("for instances with trivial details") {
      airmap::Park park;
      airmap::Airspace a1;
      a1.set_details(park);
      auto a2 = a1;
      REQUIRE(a1 == a2);
    }
    SECTION("for instances with complex details") {
      airmap::Airport airport;
      airmap::Airspace a1;
      a1.set_details(airport);
      auto a2 = a1;
      REQUIRE(a1 == a2);
    }
  }
  SECTION("instances with differences do not compare equal") {
    SECTION("for different properties") {
      airmap::Airspace a1, a2;
      a1.set_name("a1");
      a2.set_name("a2");
      REQUIRE(a1 != a2);
    }
    SECTION("for different geometries") {
      airmap::Airspace a1, a2;

      airmap::Geometry::Point p;
      p.latitude  = 42.f;
      p.longitude = 21.f;
      p.altitude  = 6.f;

      airmap::Geometry::LineString ls;
      ls.coordinates.push_back(airmap::Geometry::Coordinate{42.f, 21.f, 6.f, 6.f});
      ls.coordinates.push_back(airmap::Geometry::Coordinate{41.f, 21.f, 6.f, 6.f});

      a1.set_geometry(airmap::Geometry{p});
      a2.set_geometry(airmap::Geometry{ls});

      REQUIRE(a1 != a2);
    }
  }
}
