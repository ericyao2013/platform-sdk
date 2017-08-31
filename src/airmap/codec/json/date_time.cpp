#include <airmap/codec/json/date_time.h>

void boost::posix_time::from_json(const nlohmann::json& j, ptime& dt) {
  dt = airmap::iso8601::parse(j.get<std::string>());
}

void boost::posix_time::to_json(nlohmann::json& j, const ptime& dt) {
  j = airmap::iso8601::generate(dt);
}