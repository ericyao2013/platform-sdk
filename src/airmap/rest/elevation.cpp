#include <airmap/rest/elevation.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>
#include <airmap/net/http/middleware.h>
#include <airmap/util/fmt.h>

#include <nlohmann/json.hpp>

namespace fmt = airmap::util::fmt;
using json    = nlohmann::json;

std::string airmap::rest::Elevation::default_route_for_version(Client::Version version) {
  static constexpr const char* pattern{"/elevation/%s"};

  switch (version) {
    case airmap::Client::Version::production:
      return fmt::sprintf(pattern, "v1");
    case airmap::Client::Version::staging:
      return fmt::sprintf(pattern, "stage");
  }

  throw std::logic_error{"should not reach here"};
}

airmap::rest::Elevation::Elevation(const std::shared_ptr<net::http::Requester>& requester) : requester_{requester} {
}

void airmap::rest::Elevation::get_elevation_points(const GetElevation::Parameters& parameters, const GetElevation::Callback& cb) {
  std::unordered_map<std::string, std::string> query, headers;
  codec::http::query::encode(query, parameters);

  requester_->get("/ele/", std::move(query), std::move(headers),
                  net::http::jsend_parsing_request_callback<Report>(cb));
}