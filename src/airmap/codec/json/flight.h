
#ifndef AIRMAP_CODEC_JSON_FLIGHT_H_
#define AIRMAP_CODEC_JSON_FLIGHT_H_

#include <airmap/flight.h>

#include <airmap/codec/json/aircraft.h>
#include <airmap/codec/json/date_time.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/pilot.h>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

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

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_FLIGHT_H_