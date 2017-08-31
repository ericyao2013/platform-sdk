#include <airmap/codec/json/chrono.h>

#include <airmap/codec.h>
#include <airmap/codec/json/get.h>

void std::from_json(const nlohmann::json& j, std::chrono::seconds& s) {
  s = std::chrono::seconds{j.get<std::uint64_t>()};
}