#ifndef AIRMAP_JSEND_H_
#define AIRMAP_JSEND_H_

#include <nlohmann/json.hpp>

#include <string>

namespace airmap {
namespace jsend {
namespace status {

static constexpr const char* success{"success"};
static constexpr const char* error{"error"};
static constexpr const char* failure{"fail"};

}  // namespace status

namespace key {

static constexpr const char* status{"status"};
static constexpr const char* data{"data"};
static constexpr const char* message{"message"};

}  // namespace key

inline std::string stringify_error(const nlohmann::json& j) {
  if (j.at(key::status) == status::error) return j.at(key::data).dump();
  if (j.at(key::status) == status::failure) return j.at(key::data).dump();
  throw std::runtime_error{"not an error"};
}

}  // namespace jsend
}  // namespace airmap

#endif  // AIRMAP_JSEND_H_