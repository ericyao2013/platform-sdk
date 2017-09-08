#include <airmap/geometry.h>

#include <catch/catch.hpp>

TEST_CASE("Geometry") {
  SECTION("default ctor yields an invalid geometry") {
    airmap::Geometry geometry;
    REQUIRE(geometry.type() == airmap::Geometry::Type::invalid);
  }
  SECTION("copy ctor yields instances that compare equal") {
    airmap::Geometry g1, g2;
    REQUIRE(g1 == g2);
  }
  SECTION("assignment operator yields instances that compare equal") {
    SECTION("for default ctor") {
      airmap::Geometry g1;
      auto g2 = g1;
      REQUIRE(g1 == g2);
    }
    SECTION("for instances with trivial details") {
      airmap::Geometry::Point p;
      p.latitude  = 42.f;
      p.longitude = 21.f;
      p.altitude  = 6.f;

      airmap::Geometry g1{p};
      auto g2 = g1;
      REQUIRE(g1 == g2);
    }
    SECTION("for instances with complex details") {
      airmap::Geometry::GeometryCollection gc;

      airmap::Geometry::Point p;
      p.latitude  = 42.f;
      p.longitude = 21.f;
      p.altitude  = 6.f;

      airmap::Geometry::LineString ls;
      ls.coordinates.push_back(airmap::Geometry::Coordinate{42.f, 21.f, 6.f, 6.f});
      ls.coordinates.push_back(airmap::Geometry::Coordinate{41.f, 21.f, 6.f, 6.f});

      gc.push_back(airmap::Geometry{p});
      gc.push_back(airmap::Geometry{ls});

      airmap::Geometry g1{gc};
      auto g2 = g1;
      REQUIRE(g1 == g2);
    }
  }
}
