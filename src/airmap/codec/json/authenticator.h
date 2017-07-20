
#ifndef AIRMAP_CODEC_JSON_AUTHENTICATOR_H_
#define AIRMAP_CODEC_JSON_AUTHENTICATOR_H_

#include <airmap/authenticator.h>

#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/get.h>
#include <airmap/codec/json/optional.h>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

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

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_AUTHENTICATOR_H_