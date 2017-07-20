#ifndef AIRMAP_CODEC_HTTP_QUERY_AIRCRAFTS_H_
#define AIRMAP_CODEC_HTTP_QUERY_AIRCRAFTS_H_

#include <airmap/aircrafts.h>

#include <unordered_map>
#include <vector>

namespace airmap {
namespace codec {
namespace http {
namespace query {

inline void encode(std::unordered_map<std::string, std::string>& query,
                   const Aircrafts::Manufacturers::Parameters& parameters) {
  if (parameters.manufacturer_name) {
    query["q"] = parameters.manufacturer_name.get();
  }
}

inline void encode(std::unordered_map<std::string, std::string>& query,
                   const Aircrafts::Models::Parameters& parameters) {
  if (parameters.manufacturer) {
    query["manufacturer"] = parameters.manufacturer.get().id;
  }
  if (parameters.model_name) {
    query["q"] = parameters.model_name.get();
  }
}

}  // namespace query
}  // namespace http
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_HTTP_QUERY_AIRCRAFTS_H_
