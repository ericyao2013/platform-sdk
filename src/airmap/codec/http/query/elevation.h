#ifndef AIRMAP_CODEC_HTTP_QUERY_ELEVATION_H_
#define AIRMAP_CODEC_HTTP_QUERY_ELEVATION_H_

#include <airmap/elevation.h>

#include <unordered_map>

namespace airmap {
namespace codec {
namespace http {
namespace query {

void encode(std::unordered_map<std::string, std::string>& query, const Elevation::GetElevation::Parameters& parameters);

}  // namespace query
}  // namespace http
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_HTTP_QUERY_ELEVATION_H_
