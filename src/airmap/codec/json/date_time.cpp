#include <airmap/codec/json/date_time.h>

void airmap::from_json(const nlohmann::json& j, airmap::DateTime& dt) {
    boost::posix_time::ptime ptime;
    boost::posix_time::from_json(j, ptime);

    const std::string iso_time = boost::posix_time::to_iso_string(ptime);
    dt = airmap::DateTime(iso_time);
}

void airmap::to_json(nlohmann::json& j, const airmap::DateTime& dt) {
    const auto iso_time = dt.to_iso_string();
    const auto ptime = boost::posix_time::from_iso_string(iso_time);

    boost::posix_time::to_json(j, ptime);
}

void boost::posix_time::from_json(const nlohmann::json& j, ptime& ptime) {
  const airmap::DateTime dt = airmap::iso8601::parse(j.get<std::string>());
  const std::string time_iso = dt.to_iso_string();

  ptime = boost::posix_time::from_iso_string(time_iso);
}

void boost::posix_time::to_json(nlohmann::json& j, const ptime& ptime) {
  const std::string time_iso = boost::posix_time::to_iso_string(ptime);
  const airmap::DateTime dt(time_iso);

  j = airmap::iso8601::generate(dt);
}
