
#ifndef AIRMAP_CODEC_JSON_AIRSPACE_H_
#define AIRMAP_CODEC_JSON_AIRSPACE_H_

#include <airmap/airspace.h>

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, Airspace& airspace);
void decode(const nlohmann::json& j, std::vector<Airspace>& v);
void decode(const nlohmann::json& j, Airspace::Type& type);
void decode(const nlohmann::json& j, Airport& airport);
void decode(const nlohmann::json& j, Airport::Runway& runway);
void decode(const nlohmann::json& j, std::vector<Airport::Runway>& v);
void decode(const nlohmann::json&, Airport::Use&);
void decode(const nlohmann::json&, ControlledAirspace&);
void decode(const nlohmann::json& j, SpecialUseAirspace& sua);
void decode(const nlohmann::json&, SpecialUseAirspace::Type&);
void decode(const nlohmann::json& j, TemporaryFlightRestriction& tfr);
void decode(const nlohmann::json&, TemporaryFlightRestriction::Type&);
void decode(const nlohmann::json& j, Wildfire& wf);
void decode(const nlohmann::json&, Park&);
void decode(const nlohmann::json&, Prison&);
void decode(const nlohmann::json&, School&);
void decode(const nlohmann::json&, Hospital&);
void decode(const nlohmann::json&, Fire&);
void decode(const nlohmann::json&, Emergency&);
void decode(const nlohmann::json& j, Heliport& hp);
void decode(const nlohmann::json&, Heliport::Usage&);
void decode(const nlohmann::json& j, PowerPlant& pp);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_AIRSPACE_H_
