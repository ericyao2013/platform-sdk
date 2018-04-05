#ifndef AIRMAP_CMDS_AIRMAP_CMD_FIND_ELEVATION_H_
#define AIRMAP_CMDS_AIRMAP_CMD_FIND_ELEVATION_H_

#include <airmap/cmds/airmap/cmd/flags.h>

#include <airmap/client.h>
#include <airmap/elevation.h>
#include <airmap/logger.h>
#include <airmap/optional.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class FindElevation : public util::cli::CommandWithFlagsAndAction {
 public:
  FindElevation();

 private:
  using PointsCSV = util::TaggedString<util::tags::MustNotBeEmpty>;

  util::FormattingLogger log_{create_null_logger()};
  Client::Version version_{Client::Version::production};
  Logger::Severity log_level_{Logger::Severity::info};
  Required<ConfigFile> config_file_;
  Required<PointsCSV> points_;
  Elevation::GetElevation::Parameters params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_FIND_ELEVATION_H_
