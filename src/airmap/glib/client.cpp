#include <airmap/glib/client.h>

airmap::glib::Client::Client(const std::shared_ptr<Api>& api)
    : api_{api},
      aircrafts_{*api},
      airspaces_{*api},
      authenticator_{*api},
      flights_{*api},
      telemetry_{*api} {
}

void airmap::glib::Client::run() {
}

void airmap::glib::Client::stop() {
}

airmap::Aircrafts& airmap::glib::Client::aircrafts() {
  return aircrafts_;
}

airmap::Airspaces& airmap::glib::Client::airspaces() {
  return airspaces_;
}

airmap::Authenticator& airmap::glib::Client::authenticator() {
  return authenticator_;
}

airmap::Flights& airmap::glib::Client::flights() {
  return flights_;
}

airmap::Telemetry& airmap::glib::Client::telemetry() {
  return telemetry_;
}
