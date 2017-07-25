#ifndef AIRMAP_REST_CLIENT_H_
#define AIRMAP_REST_CLIENT_H_

#include <airmap/client.h>

#include <airmap/rest/aircrafts.h>
#include <airmap/rest/airspaces.h>
#include <airmap/rest/communicator.h>
#include <airmap/rest/authenticator.h>
#include <airmap/rest/flights.h>
#include <airmap/rest/telemetry.h>

#include <memory>

namespace airmap {
namespace rest {

class Client : public airmap::Client {
 public:
  explicit Client(const std::shared_ptr<Communicator>& communicator);

  airmap::Aircrafts& aircrafts() override;
  airmap::Airspaces& airspaces() override;
  airmap::Authenticator& authenticator() override;
  airmap::Flights& flights() override;
  airmap::Telemetry& telemetry() override;

 private:
  std::shared_ptr<rest::Communicator> communicator_;

  rest::Aircrafts aircrafts_;
  rest::Airspaces airspaces_;
  rest::Authenticator authenticator_;
  rest::Flights flights_;
  rest::Telemetry telemetry_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_CLIENT_H_
