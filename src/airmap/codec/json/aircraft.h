
#ifndef AIRMAP_CODEC_JSON_AIRCRAFT_H_
#define AIRMAP_CODEC_JSON_AIRCRAFT_H_

#include <airmap/aircraft.h>

#include <airmap/codec/json/get.h>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

inline void decode(const nlohmann::json& j, Aircraft& aircraft) {
  get(aircraft.model.id, j, "id");
  get(aircraft.model.name, j, "name");
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

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_AIRCRAFT_H_