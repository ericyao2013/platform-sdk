
#ifndef AIRMAP_CODEC_JSON_CHRONO_H_
#define AIRMAP_CODEC_JSON_CHRONO_H_

#include <airmap/codec/json/get.h>

#include <chrono>

namespace std {

inline void from_json(const nlohmann::json& j, std::chrono::seconds& s) {
  s = std::chrono::seconds{j.get<std::uint64_t>()};
}

}  // namespace std

#endif  // AIRMAP_CODEC_JSON_CHRONO_H_