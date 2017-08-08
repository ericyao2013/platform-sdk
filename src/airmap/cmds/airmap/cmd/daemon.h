#ifndef AIRMAP_CMDS_AIRMAP_CMD_DAEMON_H_
#define AIRMAP_CMDS_AIRMAP_CMD_DAEMON_H_

#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class Daemon : public util::cli::CommandWithFlagsAndAction {
 public:
  Daemon();

 private:
  using ApiKey        = util::TaggedString<util::tags::MustNotBeEmpty>;
  using UserId        = util::TaggedString<util::tags::MustNotBeEmpty>;
  using AircraftId    = util::TaggedString<util::tags::MustNotBeEmpty>;
  using SerialDevice  = util::TaggedString<util::tags::MustNotBeEmpty>;
  using UdpEndpointIp = util::TaggedString<util::tags::MustNotBeEmpty>;

  util::FormattingLogger log_{create_null_logger()};
  Optional<ApiKey> api_key_;
  Optional<UserId> user_id_;
  Optional<AircraftId> aircraft_id_;
  Optional<SerialDevice> serial_device_;
  Optional<UdpEndpointIp> udp_endpoint_ip_;
  Optional<std::uint16_t> udp_endpoint_port_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_DAEMON_H_
