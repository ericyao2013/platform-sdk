
#ifndef AIRMAP_CODEC_JSON_DATE_TIME_H_
#define AIRMAP_CODEC_JSON_DATE_TIME_H_

#include <airmap/date_time.h>

#include <nlohmann/json.hpp>

#include <string>

namespace boost {
namespace posix_time {

inline void from_json(const nlohmann::json& j, ptime& dt) {
  dt = airmap::iso8601::parse(j.get<std::string>());
}

inline void to_json(nlohmann::json& j, const ptime& dt) {
  j = airmap::iso8601::generate(dt);
}

}  // namespace posix_time
}  // namespace boost

#endif  // AIRMAP_CODEC_JSON_DATE_TIME_H_