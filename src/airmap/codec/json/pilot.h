
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
  get(p.username, j, "username");
  get(p.image, j, "image");
}

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_PILOT_H_