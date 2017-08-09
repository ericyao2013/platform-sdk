#include <airmap/daemon.h>

namespace {
constexpr const char* component{"airmap::Daemon"};
}  // namespace

airmap::Daemon::Daemon(const Configuration& configuration)
    : configuration_{configuration}, log_{configuration_.logger} {
}

airmap::Daemon::~Daemon() {
  configuration_.channel->stop();
  configuration_.channel->unsubscribe(std::move(mavlink_channel_subscription_));
  vehicle_tracker_.unregister_monitor(shared_from_this());
}

void airmap::Daemon::start() {
  auto sp = shared_from_this();

  vehicle_monitor_         = std::make_shared<mavlink::LoggingVehicleMonitor>(component, log_.logger(), sp);
  vehicle_tracker_monitor_ = std::make_shared<mavlink::LoggingVehicleTrackerMonitor>(component, log_.logger(), sp);

  vehicle_tracker_.register_monitor(vehicle_tracker_monitor_);

  mavlink_channel_subscription_ =
      configuration_.channel->subscribe([this, sp](const mavlink_message_t& msg) { handle_mavlink_message(msg); });
  configuration_.channel->start();
}

void airmap::Daemon::handle_mavlink_message(const mavlink_message_t& msg) {
  vehicle_tracker_.update(msg);
  log_.infof(component, "received messages %d, good %d, bad %d", configuration_.channel->counters().received,
             configuration_.channel->counters().good, configuration_.channel->counters().bad);
}

void airmap::Daemon::on_vehicle_added(const std::shared_ptr<mavlink::Vehicle>& vehicle) {
  vehicle->register_monitor(vehicle_monitor_);
}

void airmap::Daemon::on_vehicle_removed(const std::shared_ptr<mavlink::Vehicle>& vehicle) {
  vehicle->unregister_monitor(vehicle_monitor_);
}

void airmap::Daemon::on_system_status_changed(const Optional<mavlink::State>& old_state, mavlink::State new_state) {
}

void airmap::Daemon::on_position_changed(const Optional<mavlink::GlobalPositionInt>& old_position,
                                         const mavlink::GlobalPositionInt& new_position) {
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