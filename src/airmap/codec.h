#ifndef AIRMAP_CODEC_H_
#define AIRMAP_CODEC_H_

#include <airmap/aircraft.h>
#include <airmap/aircrafts.h>
#include <airmap/airspace.h>
#include <airmap/airspaces.h>
#include <airmap/authenticator.h>
#include <airmap/flight.h>
#include <airmap/flights.h>
#include <airmap/geometry.h>
#include <airmap/pilot.h>

#include <boost/lexical_cast.hpp>
#include <nlohmann/json.hpp>

#include <unordered_map>
#include <vector>

namespace boost {
namespace posix_time {

inline void from_json(const nlohmann::json& j, ptime& dt) {
  dt = airmap::iso8601::parse(j.get<std::string>());
}

inline void to_json(nlohmann::json& j, const ptime& dt) {
  j = airmap::iso8601::generate(dt);
}

}  // namespace posix_time
}  // namespace boost

namespace airmap {
namespace codec {
namespace json {

template <typename T>
inline void get(T& to, const nlohmann::json& from) {
  if (!from.is_null()) to = from.get<T>();
}

template <typename T>
inline void get(T& to, const nlohmann::json& from, const std::string& name) {
  if (from.count(name) > 0) get(to, from.at(name));
}

template <typename T>
inline void decode(const nlohmann::json& j, Optional<T>& o) {
  if (j.is_null()) return;

  T value{};
  get(value, j);
  o = value;
}

inline void decode(const nlohmann::json& j, Aircraft& aircraft) {
  get(aircraft.id, j, "id");
  get(aircraft.name, j, "name");
  get(aircraft.manufacturer, j, "manufacturer");
}

inline void decode(const nlohmann::json& j, std::vector<Aircraft>& aircrafts) {
  for (const auto& element : j) {
    aircrafts.emplace_back();
    aircrafts.back() = element;
  }
}

inline void decode(const nlohmann::json& j, Aircraft::Manufacturer& manufacturer) {
  get(manufacturer.id, j, "id");
  get(manufacturer.name, j, "name");
}

inline void decode(const nlohmann::json& j, std::vector<Aircraft::Manufacturer>& manufacturers) {
  for (const auto& element : j) {
    manufacturers.emplace_back();
    manufacturers.back() = element;
  }
}

inline void decode(const nlohmann::json& j, Airspace& airspace) {
  airspace.set_id(j["id"].get<std::string>());

  if (j.count("name") > 0) airspace.set_name(j["name"].get<std::string>());
  if (j.count("country") > 0) airspace.set_country(j["country"].get<std::string>());
  if (j.count("state") > 0) airspace.set_state(j["state"].get<std::string>());
  if (j.count("city") > 0 && !j["city"].is_null()) airspace.set_city(j["city"].get<std::string>());
  if (j.count("geometry") > 0) airspace.set_geometry(j["geometry"].get<Geometry>());
  if (j.count("related_geometry") > 0)
    airspace.set_related_geometries(j["related_geometry"].get<std::vector<Geometry>>());
  if (j.count("rules") > 0) airspace.set_rules(j["rules"].get<std::vector<Rule>>());
  if (j.count("last_updated") > 0) airspace.set_last_updated(iso8601::parse(j["last_updated"].get<std::string>()));
  if (j.count("type") > 0) {
    switch (j["type"].get<Airspace::Type>()) {
      case Airspace::Type::airport:
        airspace.set_details(j["properties"].get<Airport>());
        break;
      case Airspace::Type::controlled_airspace:
        airspace.set_details(j["properties"].get<ControlledAirspace>());
        break;
      case Airspace::Type::special_use_airspace:
        airspace.set_details(j["properties"].get<SpecialUseAirspace>());
        break;
      case Airspace::Type::tfr:
        airspace.set_details(j["properties"].get<TemporaryFlightRestriction>());
        break;
      case Airspace::Type::wildfire:
        airspace.set_details(j["properties"].get<Wildfire>());
        break;
      case Airspace::Type::park:
        airspace.set_details(j["properties"].get<Park>());
        break;
      case Airspace::Type::power_plant:
        airspace.set_details(j["properties"].get<PowerPlant>());
        break;
      case Airspace::Type::heliport:
        airspace.set_details(j["properties"].get<Heliport>());
        break;
      case Airspace::Type::prison:
        airspace.set_details(j["properties"].get<Prison>());
        break;
      case Airspace::Type::school:
        airspace.set_details(j["properties"].get<School>());
        break;
      case Airspace::Type::hospital:
        airspace.set_details(j["properties"].get<Hospital>());
        break;
      case Airspace::Type::fire:
        airspace.set_details(j["properties"].get<Fire>());
        break;
      case Airspace::Type::emergency:
        airspace.set_details(j["properties"].get<Emergency>());
        break;
      default:
        break;
    }
  }
}

inline void decode(const nlohmann::json& j, Airspace::Type& type) {
  auto t = j.get<std::string>();

  if (t == "airport")
    type = Airspace::Type::airport;
  else if (t == "controlled_airspace")
    type = Airspace::Type::controlled_airspace;
  else if (t == "special_use_airspace")
    type = Airspace::Type::special_use_airspace;
  else if (t == "tfr")
    type = Airspace::Type::tfr;
  else if (t == "wildfire")
    type = Airspace::Type::wildfire;
  else if (t == "park")
    type = Airspace::Type::park;
  else if (t == "power_plant")
    type = Airspace::Type::power_plant;
  else if (t == "heliport")
    type = Airspace::Type::heliport;
  else if (t == "prison")
    type = Airspace::Type::prison;
  else if (t == "school")
    type = Airspace::Type::school;
  else if (t == "hospital")
    type = Airspace::Type::hospital;
  else if (t == "fire")
    type = Airspace::Type::fire;
  else if (t == "wildfire")
    type = Airspace::Type::wildfire;
}

inline void decode(const nlohmann::json& j, std::vector<Airspace>& v) {
  for (auto element : j) {
    v.push_back(Airspace{});
    v.back() = element;
  }
}

inline void decode(const nlohmann::json& j, Authenticator::OAuthToken& token) {
  get(token.refresh, j, "refresh_token");
  get(token.id, j, "id_token");
  get(token.access, j, "access_token");
  get(token.type, j, "token_type");
}

inline void decode(const nlohmann::json& j, Authenticator::OAuthToken::Type& type) {
  auto t = j.get<std::string>();
  if (t == "bearer") type = Authenticator::OAuthToken::Type::bearer;
}

inline void decode(const nlohmann::json& j, Authenticator::AnonymousToken& token) {
  get(token.id, j, "id_token");
}

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

inline void decode(const nlohmann::json& j, std::vector<Geometry>& v) {
  for (auto element : j) {
    v.push_back(Geometry{});
    v.back() = element;
  }
}

inline void decode(const nlohmann::json& j, Rule& rule) {
  get(rule.type, j, "type");
  get(rule.id, j, "id");
  get(rule.name, j, "name");
  get(rule.description, j, "description");
  get(rule.jurisdiction, j, "jurisdiction");
}

inline void decode(const nlohmann::json& j, std::vector<Rule>& v) {
  for (auto element : j) {
    v.push_back(Rule{});
    v.back() = element;
  }
}

inline void decode(const nlohmann::json& j, Rule::Type&) {
}

inline void decode(const nlohmann::json& j, Airport& airport) {
  get(airport.iata, j, "iata");
  get(airport.icao, j, "icao");
  get(airport.paved, j, "paved");
  get(airport.phone, j, "phone");
  get(airport.tower, j, "tower");
  get(airport.runways, j, "runways");
  get(airport.elevation, j, "elevation");
  get(airport.longest_runway, j, "longest_runway");
  get(airport.use, j, "use");
}

inline void decode(const nlohmann::json& j, Airport::Runway& runway) {
  get(runway.name, j, "name");
  get(runway.length, j, "length");
  get(runway.bearing, j, "bearing");
}

inline void decode(const nlohmann::json& j, std::vector<Airport::Runway>& v) {
  for (auto element : j) {
    v.push_back(Airport::Runway{});
    v.back() = element;
  }
}

inline void decode(const nlohmann::json&, Airport::Use&) {
}
inline void decode(const nlohmann::json&, ControlledAirspace&) {
}
inline void decode(const nlohmann::json& j, SpecialUseAirspace& sua) {
  sua.type = j["type"].get<SpecialUseAirspace::Type>();
}
inline void decode(const nlohmann::json&, SpecialUseAirspace::Type&) {
}
inline void decode(const nlohmann::json& j, TemporaryFlightRestriction& tfr) {
  get(tfr.url, j, "url");
  get(tfr.type, j, "type");
  get(tfr.reason, j, "notam_reason");
}

inline void decode(const nlohmann::json&, TemporaryFlightRestriction::Type&) {
}

inline void decode(const nlohmann::json& j, Wildfire& wf) {
  get(wf.effective_date, j, "date_effective");
}

inline void decode(const nlohmann::json&, Park&) {
}
inline void decode(const nlohmann::json&, Prison&) {
}
inline void decode(const nlohmann::json&, School&) {
}
inline void decode(const nlohmann::json&, Hospital&) {
}
inline void decode(const nlohmann::json&, Fire&) {
}
inline void decode(const nlohmann::json&, Emergency&) {
}

inline void decode(const nlohmann::json& j, Heliport& hp) {
  get(hp.faa_id, j, "faa");
  get(hp.phone, j, "phone");
  get(hp.usage, j, "usage");
}

inline void decode(const nlohmann::json&, Heliport::Usage&) {
}

inline void decode(const nlohmann::json& j, PowerPlant& pp) {
  get(pp.technology, j, "tech");
  get(pp.code, j, "plant_code");
}

inline void decode(const nlohmann::json& j, Flight& f) {
  get(f.id, j, "id");
  get(f.latitude, j, "latitude");
  get(f.longitude, j, "longitude");
  get(f.max_altitude, j, "max_altitude");
  get(f.geometry, j, "geometry");
  get(f.created_at, j, "created_at");
  get(f.start_time, j, "start_time");
  get(f.end_time, j, "end_time");
  get(f.pilot, j, "pilot");
  get(f.pilot.id, j, "pilot_id");
  get(f.aircraft, j, "aircraft");
  get(f.aircraft.id, j, "aircraft_id");
}

inline void decode(const nlohmann::json& j, std::vector<Flight>& f) {
  for (const auto& element : j) {
    f.emplace_back();
    f.back() = element;
  }
}

inline void decode(const nlohmann::json& j, Flights::EndFlight::Response& response) {
  get(response.end_time, j, "end_time");
}

inline void decode(const nlohmann::json& j, Flights::DeleteFlight::Response& response) {
  get(response.id, j, "id");
}

inline void decode(const nlohmann::json& j, Flights::StartFlightCommunications::Response& response) {
  get(response.key, j, "key");
}

inline void decode(const nlohmann::json& j, Flights::EndFlightCommunications::Response& response) {
}

inline void decode(const nlohmann::json& j, Pilot& p) {
  get(p.id, j, "id");
  get(p.username, j, "username");
  get(p.image, j, "image");
}

inline void encode(nlohmann::json&, const Airspace::Type&) {
}
inline void encode(nlohmann::json&, const Airspaces::Search::Parameters&) {
}

inline void encode(nlohmann::json& j, const Authenticator::AuthenticateWithPassword::Params& params) {
  j["client_id"] = params.client_id;
  j["connection"] = params.connection_name;
  j["username"] = params.username;
  j["password"] = params.password;
  j["device"] = params.device;
}

inline void encode(nlohmann::json& j, const Authenticator::AuthenticateAnonymously::Params& params) {
  j["user_id"] = params.user_id;
}

inline void encode(nlohmann::json& j, const Flights::CreateFlight::Parameters& parameters) {
  j["latitude"] = parameters.latitude;
  j["longitude"] = parameters.longitude;
  j["max_altitude"] = parameters.max_altitude;
  j["aircraft_id"] = parameters.aircraft_id;
  j["start_time"] = parameters.start_time;
  j["end_time"] = parameters.end_time;
  j["public"] = parameters.is_public;
  j["notify"] = parameters.give_digital_notice;
  j["buffer"] = parameters.buffer;
  if (parameters.geometry) {
    j["geometry"] = parameters.geometry.get();
  }
}

inline void encode(nlohmann::json& j, const Geometry& geometry) {
  j["type"] = geometry.type();
  switch (geometry.type()) {
    case Geometry::Type::point:
      j["coordinates"] = geometry.details_for_point();
      break;
    case Geometry::Type::multi_point:
      j["coordinates"] = geometry.details_for_multi_point();
      break;
    case Geometry::Type::line_string:
      j["coordinates"] = geometry.details_for_line_string();
      break;
    case Geometry::Type::multi_line_string:
      j["coordinates"] = geometry.details_for_multi_line_string();
      break;
    case Geometry::Type::polygon:
      j["coordinates"] = geometry.details_for_polygon();
      break;
    case Geometry::Type::multi_polygon:
      j["coordinates"] = geometry.details_for_multi_polygon();
      break;
    case Geometry::Type::geometry_collection:
      j["coordinates"] = geometry.details_for_geometry_collection();
      break;
  }
}

inline void encode(nlohmann::json& j, const std::vector<Geometry>& geometries) {
  for (const auto& geometry : geometries) j.push_back(geometry);
}

inline void encode(nlohmann::json& j, Geometry::Type type) {
  switch (type) {
    case Geometry::Type::point:
      j = "Point";
      break;
    case Geometry::Type::multi_point:
      j = "MultiPoint";
      break;
    case Geometry::Type::line_string:
      j = "LineString";
      break;
    case Geometry::Type::multi_line_string:
      j = "MultiLineString";
      break;
    case Geometry::Type::polygon:
      j = "Polygon";
      break;
    case Geometry::Type::multi_polygon:
      j = "MultiPolygon";
      break;
    case Geometry::Type::geometry_collection:
      j = "GeometryCollection";
      break;
  }
}

inline void encode(nlohmann::json& j, const Geometry::Coordinate& coordinate) {
  j.push_back(coordinate.longitude);
  j.push_back(coordinate.latitude);
  if (coordinate.altitude) j.push_back(coordinate.altitude.get(0.f));
  if (coordinate.elevation) j.push_back(coordinate.elevation.get(0.f));
}

inline void encode(nlohmann::json& j, const std::vector<Geometry::Coordinate>& coordinates) {
  for (const auto& coordinate : coordinates) j.push_back(coordinate);
}

template <Geometry::Type tag>
inline void encode(nlohmann::json& j, const Geometry::CoordinateVector<tag>& cv) {
  j = cv.coordinates;
}

template <Geometry::Type tag>
inline void encode(nlohmann::json& j, const std::vector<Geometry::CoordinateVector<tag>>& cvs) {
  for (const auto& cv : cvs) j.push_back(cv);
}

inline void encode(nlohmann::json& j, const std::vector<Geometry::Polygon>& cvs) {
  for (const auto& cv : cvs) j.push_back(cv);
}

}  // namespace json

namespace http {
namespace query {

inline void encode(std::unordered_map<std::string, std::string>& query,
                   const Aircrafts::Manufacturers::Parameters& parameters) {
  if (parameters.manufacturer_name) {
    query["q"] = parameters.manufacturer_name.get(std::string{});
  }
}

inline void encode(std::unordered_map<std::string, std::string>& query,
                   const Aircrafts::Models::Parameters& parameters) {
  if (parameters.manufacturer) {
    query["manufacturer"] = parameters.manufacturer.get(airmap::Aircraft::Manufacturer{}).id;
  }
  if (parameters.model_name) {
    query["q"] = parameters.model_name.get(std::string{});
  }
}

inline void encode(std::unordered_map<std::string, std::string>& query,
                   const Airspaces::Search::Parameters& parameters) {
  if (parameters.types != Airspace::Type::invalid) {
    std::ostringstream oss;
    oss << ((parameters.types & ~Airspace::Type::emergency) & ~Airspace::Type::fire);
    query["types"] = oss.str();
  }

  if (parameters.ignored_types && parameters.ignored_types.get(Airspace::Type::invalid) != Airspace::Type::invalid) {
    std::ostringstream oss;
    oss << ((parameters.ignored_types.get(Airspace::Type::invalid) & ~Airspace::Type::emergency) &
            ~Airspace::Type::fire);
    query["ignored_types"] = oss.str();
  }

  query["full"] = parameters.full ? "true" : "false";
  nlohmann::json geometry;
  geometry = parameters.geometry;
  query["geometry"] = geometry.dump();

  if (parameters.buffer) query["buffer"] = boost::lexical_cast<std::string>(parameters.buffer.get(0));
  if (parameters.offset) query["offset"] = boost::lexical_cast<std::string>(parameters.offset.get(0));
  if (parameters.date_time) query["datetime"] = iso8601::generate(parameters.date_time.get(Clock::universal_time()));
}

inline void encode(std::unordered_map<std::string, std::string>& query, const Flights::ForId::Parameters& parameters) {
  if (parameters.enhance) query["enhance"] = parameters.enhance.get() ? "true" : "false";
}

inline void encode(std::unordered_map<std::string, std::string>& query, const Flights::Search::Parameters& parameters) {
  if (parameters.limit) query["limit"] = boost::lexical_cast<std::string>(parameters.limit.get());
  if (parameters.geometry) {
    nlohmann::json geometry;
    geometry = parameters.geometry.get();
    query["geometry"] = geometry.dump();
  }
  if (parameters.country) query["country"] = parameters.country.get();
  if (parameters.state) query["state"] = parameters.state.get();
  if (parameters.city) query["city"] = parameters.city.get();
  if (parameters.pilot_id) query["pilot_id"] = parameters.pilot_id.get();
  if (parameters.start_after) query["start_after"] = iso8601::generate(parameters.start_after.get());
  if (parameters.start_before) query["start_before"] = iso8601::generate(parameters.start_before.get());
  if (parameters.end_after) query["end_after"] = iso8601::generate(parameters.end_after.get());
  if (parameters.end_before) query["end_before"] = iso8601::generate(parameters.end_before.get());
  if (parameters.enhance) query["enhance"] = parameters.enhance.get() ? "true" : "false";
}

}  // namespace query
}  // namespace http
}  // namespace codec

template <typename T>
inline void from_json(const nlohmann::json& j, T& value) {
  codec::json::decode(j, value);
}

template <typename T>
inline void to_json(nlohmann::json& j, const T& value) {
  codec::json::encode(j, value);
}

}  // namespace airmap

#endif  // AIRMAP_CODEC_H_
