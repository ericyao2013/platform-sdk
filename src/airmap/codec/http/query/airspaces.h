#ifndef AIRMAP_CODEC_HTTP_QUERY_AIRSPACES_H_
#define AIRMAP_CODEC_HTTP_QUERY_AIRSPACES_H_

#include <airmap/airspaces.h>

#include <airmap/codec/json/geometry.h>

#include <boost/lexical_cast.hpp>

#include <sstream>
#include <unordered_map>

namespace airmap {
namespace codec {
namespace http {
namespace query {

inline void encode(std::unordered_map<std::string, std::string>& query,
                   const Airspaces::Search::Parameters& parameters) {
  if (parameters.types && parameters.types.get() != Airspace::Type::invalid) {
    query["types"] =
        boost::lexical_cast<std::string>((parameters.types.get() & ~Airspace::Type::emergency) & ~Airspace::Type::fire);
  }
  if (parameters.ignored_types && parameters.ignored_types.get() != Airspace::Type::invalid) {
    query["ignored_types"] = boost::lexical_cast<std::string>(
        (parameters.ignored_types.get() & ~Airspace::Type::emergency) & ~Airspace::Type::fire);
  }

  query["full"] = parameters.full ? "true" : "false";
  nlohmann::json geometry;
  geometry          = parameters.geometry;
  query["geometry"] = geometry.dump();

  if (parameters.buffer)
    query["buffer"] = boost::lexical_cast<std::string>(parameters.buffer.get());
  if (parameters.offset)
    query["offset"] = boost::lexical_cast<std::string>(parameters.offset.get());
  if (parameters.date_time)
    query["datetime"] = iso8601::generate(parameters.date_time.get());
}

}  // namespace query
}  // namespace http
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_HTTP_QUERY_AIRSPACES_H_
