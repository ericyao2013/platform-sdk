#ifndef AIRMAP_DAEMON_H_
#define AIRMAP_DAEMON_H_

#include <airmap/authenticator.h>
#include <airmap/client.h>
#include <airmap/flights.h>
#include <airmap/logger.h>
#include <airmap/mavlink/channel.h>
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
class Daemon : public std::enable_shared_from_this<Daemon> {
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

 private:
  void handle_mavlink_message(const mavlink_message_t& msg);
  void handle_mavlink_heartbeat_message(const mavlink_message_t& msg);
  void handle_mavlink_system_status_message(const mavlink_message_t& msg);
  void handle_mavlink_gps_raw_int_message(const mavlink_message_t& msg);

  void handle_authorized(const Authenticator::AuthenticateAnonymously::Result& result);
  void handle_flight(const airmap::Flights::CreateFlight::Result& result);
  void handle_flight_comms_started(const airmap::Flights::StartFlightCommunications::Result& result);

  Configuration configuration_;

  util::FormattingLogger log_;
  mavlink::Channel::Subscription mavlink_channel_subscription_;
  MAV_STATE state_{MAV_STATE_UNINIT};
  Optional<std::string> flight_id_;
  Optional<std::string> authorization_;
  Optional<std::string> encryption_key_;
};

}  // namespace airmap

#endif  // AIRMAP_DAEMON_H_
