#ifndef AIRMAP_CMDS_AIRMAP_CMD_SIMULATE_TELEMETRY_H_
#define AIRMAP_CMDS_AIRMAP_CMD_SIMULATE_TELEMETRY_H_

#include <airmap/flight.h>
#include <airmap/flights.h>
#include <airmap/telemetry.h>
#include <airmap/util/cli.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class SimulateTelemetry : public util::cli::CommandWithFlagsAndAction {
 public:
  SimulateTelemetry();

 private:
  struct {
    std::string api_key;
    std::string authorization;
    std::string flight_id;
    std::string host{"127.0.0.1"};
    std::uint16_t port{16060};
    std::uint16_t frequency{5};
    Flight flight{};
    std::string encryption_key{"MDAwMDExMTEyMjIyMzMzMzQ0NDQ1NTU1NjY2Njc3Nzc="};
  } params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_SIMULATE_TELEMETRY_H_