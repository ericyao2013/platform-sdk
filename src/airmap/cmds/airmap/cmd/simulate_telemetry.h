#ifndef AIRMAP_CMDS_AIRMAP_CMD_SIMULATE_TELEMETRY_H_
#define AIRMAP_CMDS_AIRMAP_CMD_SIMULATE_TELEMETRY_H_

#include <airmap/client.h>
#include <airmap/flight.h>
#include <airmap/flights.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/telemetry.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class SimulateTelemetry : public util::cli::CommandWithFlagsAndAction {
 public:
  SimulateTelemetry();

 private:
  using ApiKey        = util::TaggedString<util::tags::MustNotBeEmpty>;
  using Authorization = util::TaggedString<util::tags::MustNotBeEmpty>;
  using Host          = util::TaggedString<util::tags::MustNotBeEmpty>;
  using EncryptionKey = util::TaggedString<util::tags::MustNotBeEmpty>;
  using GeometryFile  = util::TaggedString<util::tags::MustNotBeEmpty>;

  util::FormattingLogger log_{create_null_logger()};
  struct {
    Client::Version version{Client::Version::production};
    Optional<ApiKey> api_key;
    Optional<Authorization> authorization;
    Optional<Host> host;
    Optional<std::uint16_t> port;
    std::uint16_t frequency{5};
    Flight flight{};
    Optional<EncryptionKey> encryption_key{EncryptionKey{"MDAwMDExMTEyMjIyMzMzMzQ0NDQ1NTU1NjY2Njc3Nzc="}};
    Optional<GeometryFile> geometry_file;
  } params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_SIMULATE_TELEMETRY_H_