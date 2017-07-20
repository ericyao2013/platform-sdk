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

#include <airmap/codec/json/aircraft.h>
#include <airmap/codec/json/airspace.h>
#include <airmap/codec/json/authenticator.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/flight.h>
#include <airmap/codec/json/flights.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>
#include <airmap/codec/json/pilot.h>
#include <airmap/codec/json/rule.h>

#include <boost/lexical_cast.hpp>

#include <unordered_map>
#include <vector>

namespace airmap {
namespace codec {
namespace http {
namespace query {

inline void encode(std::unordered_map<std::string, std::string>& query,
                   const Aircrafts::Manufacturers::Parameters& parameters) {
  if (parameters.manufacturer_name) {
    query["q"] = parameters.manufacturer_name.get();
  }
}

inline void encode(std::unordered_map<std::string, std::string>& query,
                   const Aircrafts::Models::Parameters& parameters) {
  if (parameters.manufacturer) {
    query["manufacturer"] = parameters.manufacturer.get().id;
  }
  if (parameters.model_name) {
    query["q"] = parameters.model_name.get();
  }
}

inline void encode(std::unordered_map<std::string, std::string>& query,
                   const Airspaces::Search::Parameters& parameters) {
  if (parameters.types != Airspace::Type::invalid) {
    std::ostringstream oss;
    oss << ((parameters.types & ~Airspace::Type::emergency) & ~Airspace::Type::fire);
    query["types"] = oss.str();
  }

  if (parameters.ignored_types && parameters.ignored_types.get() != Airspace::Type::invalid) {
    std::ostringstream oss;
    oss << ((parameters.ignored_types.get() & ~Airspace::Type::emergency) & ~Airspace::Type::fire);
    query["ignored_types"] = oss.str();
  }

  query["full"] = parameters.full ? "true" : "false";
  nlohmann::json geometry;
  geometry = parameters.geometry;
  query["geometry"] = geometry.dump();

  if (parameters.buffer) query["buffer"] = boost::lexical_cast<std::string>(parameters.buffer.get());
  if (parameters.offset) query["offset"] = boost::lexical_cast<std::string>(parameters.offset.get());
  if (parameters.date_time) query["datetime"] = iso8601::generate(parameters.date_time.get());
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
