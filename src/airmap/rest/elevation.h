#ifndef AIRMAP_REST_ELEVATION_H_
#define AIRMAP_REST_ELEVATION_H_

#include <airmap/client.h>
#include <airmap/elevation.h>
#include <airmap/net/http/requester.h>

namespace airmap {
namespace rest {

class Elevation : public airmap::Elevation {
 public:
  static std::string default_route_for_version(Client::Version version);

  explicit Elevation(const std::shared_ptr<net::http::Requester>& requester);

  void get_elevation_points(const GetElevation::Parameters& parameters, const GetElevation::Callback& cb) override;

 private:
  std::shared_ptr<net::http::Requester> requester_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_ELEVATION_H_
