
#ifndef AIRMAP_CODEC_JSON_FLIGHTS_H_
#define AIRMAP_CODEC_JSON_FLIGHTS_H_

#include <airmap/flights.h>

#include <airmap/codec/json/date_time.h>

#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

#include <string>

namespace airmap {
namespace codec {
namespace json {

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

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_FLIGHTS_H_