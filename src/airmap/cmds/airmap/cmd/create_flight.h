#ifndef AIRMAP_CMDS_AIRMAP_CMD_CREATE_FLIGHT_H_
#define AIRMAP_CMDS_AIRMAP_CMD_CREATE_FLIGHT_H_

#include <airmap/flights.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class CreateFlight : public util::cli::CommandWithFlagsAndAction {
 public:
  CreateFlight();

 private:
  using ApiKey       = util::TaggedString<util::tags::MustNotBeEmpty>;
  using Authorization = util::TaggedString<util::tags::MustNotBeEmpty>;
  using GeometryFile = util::TaggedString<util::tags::MustNotBeEmpty>;

  util::FormattingLogger log_{create_null_logger()};
  Optional<ApiKey> api_key_;
  Optional<Authorization> authorization_;
  Optional<GeometryFile> geometry_file_;
  Flights::CreateFlight::Parameters params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_CREATE_FLIGHT_H_
