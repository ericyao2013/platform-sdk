#include <airmap/daemon.h>

namespace {
constexpr const char* component{"airmap::Daemon"};
}  // namespace

airmap::Daemon::Daemon(const Configuration& configuration) : configuration_{configuration}, log_{configuration_.logger} {
}

airmap::Daemon::~Daemon() {
  configuration_.channel->cancel();
  configuration_.channel->unsubscribe(
    std::move(mavlink_channel_subscription_)
  );
}

void airmap::Daemon::start() {
  auto sp = shared_from_this();
  mavlink_channel_subscription_ = configuration_.channel->subscribe([this, sp](const mavlink_message_t& msg) {
    handle_mavlink_message(msg);
  });
  configuration_.channel->start();
}

void airmap::Daemon::handle_mavlink_message(const mavlink_message_t& msg) {
  log_.infof(component, "receiving mavlink message: %d", msg.msgid);
  start();
}

void airmap::Daemon::handle_authorized(const Authenticator::AuthenticateAnonymously::Result& result) {
  if (result) {
    authorization_ = result.value().id;
  }
}

void airmap::Daemon::handle_flight(const airmap::Flights::CreateFlight::Result& result) {
  if (result) {
    flight_id_ = result.value().id;
  }
}

void airmap::Daemon::handle_flight_comms_started(const airmap::Flights::StartFlightCommunications::Result& result) {
  if (result) {
    encryption_key_ = result.value().key;
  }
}