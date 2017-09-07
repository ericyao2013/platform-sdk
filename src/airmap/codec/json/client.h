
#ifndef AIRMAP_CODEC_JSON_CLIENT_H_
#define AIRMAP_CODEC_JSON_CLIENT_H_

#include <airmap/client.h>

#include <nlohmann/json.hpp>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

void decode(const nlohmann::json& j, Client::Configuration& configuration);
void decode(const nlohmann::json& j, Client::Version& version);

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_CLIENT_H_
