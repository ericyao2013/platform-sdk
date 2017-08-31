
#ifndef AIRMAP_CODEC_JSON_DATE_TIME_H_
#define AIRMAP_CODEC_JSON_DATE_TIME_H_

#include <airmap/date_time.h>

#include <nlohmann/json.hpp>

#include <string>

namespace boost {
namespace posix_time {

void from_json(const nlohmann::json& j, ptime& dt);
void to_json(nlohmann::json& j, const ptime& dt);

}  // namespace posix_time
}  // namespace boost

#endif  // AIRMAP_CODEC_JSON_DATE_TIME_H_