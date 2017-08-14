
#ifndef AIRMAP_CODEC_JSON_PILOT_H_
#define AIRMAP_CODEC_JSON_PILOT_H_

#include <airmap/pilot.h>

#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

#include <string>

namespace airmap {
namespace codec {
namespace json {

inline void decode(const nlohmann::json& j, Pilot& p) {
  get(p.id, j, "id");
  get(p.first_name, j, "first_name");
  get(p.last_name, j, "last_name");
  get(p.user_name, j, "username");
  get(p.picture_url, j, "picture_url");
  get(p.created_at, j, "created_at");
  get(p.verification_status, j, "verification_status");
  get(p.statistics, j, "statistics");
  get(p.metadata.app, j, "app_metadata");
  get(p.metadata.user, j, "user_metadata");
}

inline void decode(const nlohmann::json& j, Pilot::VerificationStatus& s) {
  get(s.email, j, "email");
  get(s.phone, j, "phone");
}

inline void decode(const nlohmann::json& j, Pilot::Statistics& s) {
  get(s.flight, j, "flight");
  get(s.aircraft, j, "aircraft");
}

inline void decode(const nlohmann::json& j, Pilot::Statistics::Flight& f) {
  get(f.total, j, "total");
  get(f.last_flight_time, j, "last_flight_time");
}

inline void decode(const nlohmann::json& j, Pilot::Statistics::Aircraft& a) {
  get(a.total, j, "total");
}

inline void decode(const nlohmann::json& j, Pilot::Aircraft& a) {
  get(a.id, j, "id");
  get(a.nick_name, j, "nickname");
  get(a.created_at, j, "created_at");
  get(a.model, j, "model");
}

inline void decode(const nlohmann::json& j, std::vector<Pilot::Aircraft>& v) {
  for (auto element : j) {
    v.push_back(Pilot::Aircraft{});
    v.back() = element;
  }
}

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_PILOT_H_