
#ifndef AIRMAP_CODEC_JSON_RULE_H_
#define AIRMAP_CODEC_JSON_RULE_H_

#include <airmap/rule.h>

#include <airmap/codec/json/get.h>

#include <vector>

namespace airmap {
namespace codec {
namespace json {

inline void decode(const nlohmann::json& j, Rule& rule) {
  get(rule.type, j, "type");
  get(rule.id, j, "id");
  get(rule.name, j, "name");
  get(rule.description, j, "description");
  get(rule.jurisdiction, j, "jurisdiction");
}

inline void decode(const nlohmann::json& j, std::vector<Rule>& v) {
  for (auto element : j) {
    v.push_back(Rule{});
    v.back() = element;
  }
}

inline void decode(const nlohmann::json&, Rule::Type&) {
}

}  // namespace json
}  // namespace codec
}  // namespace airmap

#endif  // AIRMAP_CODEC_JSON_RULE_H_