
#ifndef AIRMAP_CODEC_JSON_GEOMETRY_H_
#define AIRMAP_CODEC_JSON_GEOMETRY_H_

#include <airmap/geometry.h>

#include <airmap/codec/json/get.h>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

inline void decode(const nlohmann::json& j, Geometry& g) {
  auto type = j["type"].get<Geometry::Type>();

  switch (type) {
    case Geometry::Type::point: {
      g = Geometry{j["coordinates"].get<Geometry::Point>()};
      break;
    }
    case Geometry::Type::multi_point: {
      g = Geometry{j["coordinates"].get<Geometry::MultiPoint>()};
      break;
    }
    case Geometry::Type::line_string: {
      g = Geometry{j["coordinates"].get<Geometry::LineString>()};
      break;
    }
    case Geometry::Type::multi_line_string: {
      g = Geometry{j["coordinates"].get<Geometry::MultiLineString>()};
      break;
    }
    case Geometry::Type::polygon: {
      g = Geometry{j["coordinates"].get<Geometry::Polygon>()};
      break;
    }
    case Geometry::Type::multi_polygon: {
      g = Geometry{j["coordinates"].get<Geometry::MultiPolygon>()};
      break;
    }
    case Geometry::Type::geometry_collection: {
      g = Geometry{j["coordinates"].get<Geometry::GeometryCollection>()};
      break;
    }
  }
}

inline void decode(const nlohmann::json& j, std::vector<Geometry>& v) {
  for (auto element : j) {
    v.push_back(Geometry{});
    v.back() = element;
  }
}

inline void decode(const nlohmann::json& j, Geometry::Type& t) {
  const auto& type_string = j.get<std::string>();

  if (type_string == "Point")
    t = Geometry::Type::point;
  else if (type_string == "MultiPoint")
    t = Geometry::Type::multi_point;
  else if (type_string == "LineString")
    t = Geometry::Type::line_string;
  else if (type_string == "MultiLineString")
    t = Geometry::Type::multi_line_string;
  else if (type_string == "Polygon")
    t = Geometry::Type::polygon;
  else if (type_string == "MultiPolygon")
    t = Geometry::Type::multi_polygon;
  else if (type_string == "GeometryCollection")
    t = Geometry::Type::geometry_collection;
}

inline void decode(const nlohmann::json& j, Geometry::Coordinate& c) {
  c.latitude = j.at(0).get<float>();
  c.longitude = j.at(1).get<float>();
  if (j.size() > 2) c.altitude = j.at(2).get<float>();
}

template <Geometry::Type tag>
inline void decode(const nlohmann::json& j, Geometry::CoordinateVector<tag>& cv) {
  for (const auto& element : j) {
    cv.coordinates.push_back(Geometry::Point{});
    cv.coordinates.back() = element;
  }
}

inline void decode(const nlohmann::json& j, Geometry::MultiLineString& mls) {
  for (const auto& element : j) {
    mls.push_back(Geometry::LineString{});
    mls.back() = element;
  }
}

inline void decode(const nlohmann::json& j, Geometry::Polygon& p) {
  for (const auto& element : j) {
    p.push_back(Geometry::CoordinateVector<Geometry::Type::polygon>{});
    p.back() = element;
  }
}

inline void decode(const nlohmann::json& j, Geometry::MultiPolygon& mp) {
  for (const auto& element : j) {
    mp.push_back(Geometry::Polygon{});
    mp.back() = element.get<Geometry::Polygon>();
  }
}

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_GEOMETRY_H_