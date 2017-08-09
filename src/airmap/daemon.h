#ifndef AIRMAP_DAEMON_H_
#define AIRMAP_DAEMON_H_

#include <airmap/authenticator.h>
#include <airmap/client.h>
#include <airmap/flights.h>
#include <airmap/logger.h>
#include <airmap/mavlink/channel.h>
#include <airmap/mavlink/vehicle.h>
#include <airmap/mavlink/vehicle_tracker.h>
#include <airmap/optional.h>
#include <airmap/telemetry.h>
#include <airmap/util/formatting_logger.h>

#include <memory>

namespace airmap {

/// Daemon respresents AirMap and all of its services within
/// the system. It listens to incoming data (both in terms of
/// sensors as well as in terms of control messages) and enables
/// a drone operation platform to deeply integrate airspace intelligence
/// in its operations.
class Daemon : public mavlink::VehicleTracker::Monitor,
               public mavlink::Vehicle::Monitor,
               public std::enable_shared_from_this<Daemon> {
 public:
  struct Configuration {
    std::string api_key;
    std::string user_id;
    std::string aircraft_id;
    std::shared_ptr<Logger> logger;
    std::shared_ptr<mavlink::Channel> channel;
    std::shared_ptr<Client> client;
  };

  /// Daemon initializes a new instance with the functional
  /// dependencies provided via 'configuration'.
  explicit Daemon(const Configuration& configuration);
  ~Daemon();

  /// Puts the daemon to execution.
  void start();

 protected:
  // From VehicleTracker::Monitor
  void on_vehicle_added(const std::shared_ptr<mavlink::Vehicle>& vehicle) override;
  void on_vehicle_removed(const std::shared_ptr<mavlink::Vehicle>& vehicle) override;
  // From Vehicle::Monitor
  void on_system_status_changed(const Optional<MAV_STATE>& old_state, MAV_STATE new_state) override;
  void on_position_changed(const Optional<mavlink_global_position_int_t>& old_position,
                           const mavlink_global_position_int_t& new_position) override;

 private:
  void handle_mavlink_message(const mavlink_message_t& msg);

  void handle_authorized(const Authenticator::AuthenticateAnonymously::Result& result);
  void handle_flight(const airmap::Flights::CreateFlight::Result& result);
  void handle_flight_comms_started(const airmap::Flights::StartFlightCommunications::Result& result);

  Configuration configuration_;

  util::FormattingLogger log_;
  mavlink::VehicleTracker vehicle_tracker_;
  mavlink::Channel::Subscription mavlink_channel_subscription_;
  Optional<std::string> flight_id_;
  Optional<std::string> authorization_;
  Optional<std::string> encryption_key_;
};

}  // namespace airmap

#endif  // AIRMAP_DAEMON_H_
