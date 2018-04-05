#include <airmap/codec/http/query/elevation.h>

#include <airmap/codec.h>
#include <airmap/codec/json/geometry.h>
#include <airmap/date_time.h>

#include <boost/lexical_cast.hpp>

#include <sstream>

void airmap::codec::http::query::encode(std::unordered_map<std::string, std::string>& query,
                                        const Elevation::GetElevation::Parameters& parameters) {
  // if (!parameters.points.empty()) {
  //   std::ostringstream oss;
  //   std::copy(parameters.points.begin(), parameters.points.end()-1, std::ostream_iterator<float>(oss, ","));
  //   oss << parameters.points.back();
  //   query["points"] = oss.str();
  // }
  if (parameters.points)
    query["points"] = parameters.points.get();
}