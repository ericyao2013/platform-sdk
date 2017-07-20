#ifndef AIRMAP_GLIB_CLIENT_H_
#define AIRMAP_GLIB_CLIENT_H_

#include <airmap/client.h>

#include <airmap/glib/aircrafts.h>
#include <airmap/glib/airspaces.h>
#include <airmap/glib/api.h>
#include <airmap/glib/authenticator.h>
#include <airmap/glib/flights.h>
#include <airmap/glib/telemetry.h>

#include <memory>

namespace airmap {
namespace glib {

class Client : public airmap::Client {
 public:
  explicit Client(const std::shared_ptr<Api>& api);

  void run() override;
  void stop() override;
  airmap::Aircrafts& aircrafts() override;
  airmap::Airspaces& airspaces() override;
  airmap::Authenticator& authenticator() override;
  airmap::Flights& flights() override;
  airmap::Telemetry& telemetry() override;

 private:
  std::shared_ptr<glib::Api> api_;

  glib::Aircrafts aircrafts_;
  glib::Airspaces airspaces_;
  glib::Authenticator authenticator_;
  glib::Flights flights_;
  glib::Telemetry telemetry_;
};

}  // namespace glib
}  // namespace airmap

#endif  // AIRMAP_GLIB_CLIENT_H_
