
#ifndef AIRMAP_CODEC_JSON_PILOTS_H_
#define AIRMAP_CODEC_JSON_PILOTS_H_

#include <airmap/pilots.h>

#include <airmap/codec/json/date_time.h>

#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

#include <string>

namespace airmap {
namespace codec {
namespace json {

inline void encode(nlohmann::json& j, const Pilots::UpdateForId::Parameters& parameters) {
  j["first_name"] = parameters.first_name;
  j["last_name"]  = parameters.last_name;
  j["username"]   = parameters.user_name;
  j["phone"]      = parameters.phone;
}

inline void encode(nlohmann::json& j, const Pilots::FinishVerifyPilotPhoneForId::Parameters& parameters) {
  j["token"] = parameters.token;
}

inline void encode(nlohmann::json& j, const Pilots::AddAircraft::Parameters& parameters) {
  j["model_id"]  = parameters.model_id;
  j["nick_name"] = parameters.nick_name;
}

inline void encode(nlohmann::json& j, const Pilots::DeleteAircraft::Parameters& parameters) {
  j["aircraft_id"] = parameters.aircraft_id;
}

inline void encode(nlohmann::json& j, const Pilots::UpdateAircraft::Parameters& parameters) {
  j["nick_name"] = parameters.nick_name;
}

inline void decode(const nlohmann::json&, Pilots::StartVerifyPilotPhoneForId::Empty&) {
}

inline void decode(const nlohmann::json&, Pilots::FinishVerifyPilotPhoneForId::Empty&) {
}

inline void decode(const nlohmann::json&, Pilots::DeleteAircraft::Empty&) {
}

inline void decode(const nlohmann::json&, Pilots::UpdateAircraft::Empty&) {
}

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_PILOTS_H_