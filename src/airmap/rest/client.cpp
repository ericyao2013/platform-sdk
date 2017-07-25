#include <airmap/rest/client.h>

airmap::rest::Client::Client(const std::shared_ptr<Communicator>& communicator)
    : communicator_{communicator},
      aircrafts_{*communicator},
      airspaces_{*communicator},
      authenticator_{*communicator},
      flights_{*communicator},
      telemetry_{*communicator} {
}

airmap::Aircrafts& airmap::rest::Client::aircrafts() {
  return aircrafts_;
}

airmap::Airspaces& airmap::rest::Client::airspaces() {
  return airspaces_;
}

airmap::Authenticator& airmap::rest::Client::authenticator() {
  return authenticator_;
}

airmap::Flights& airmap::rest::Client::flights() {
  return flights_;
}

airmap::Telemetry& airmap::rest::Client::telemetry() {
  return telemetry_;
}
