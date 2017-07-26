#ifndef AIRMAP_CMDS_AIRMAP_CMD_START_FLIGHT_COMMS_H_
#define AIRMAP_CMDS_AIRMAP_CMD_START_FLIGHT_COMMS_H_

#include <airmap/flight.h>
#include <airmap/flights.h>
#include <airmap/telemetry.h>
#include <airmap/util/cli.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class StartFlightComms : public util::cli::CommandWithFlagsAndAction {
 public:
  StartFlightComms();

 private:
  struct {
    std::string api_key;
    std::string authorization;
    std::string flight_id;
  } params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_START_FLIGHT_COMMS_H_