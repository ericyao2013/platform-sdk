#include <airmap/codec.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/codec/json/rulesets.h>

void airmap::codec::json::encode(nlohmann::json& j, const RuleSets::Search::Parameters& p) {
  j["geometry"] = p.geometry;
}
