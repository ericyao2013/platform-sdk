#include <airmap/cmds/airmap/cmd/version.h>

#include <airmap/util/formatting_logger.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {
constexpr const char* component{"get-status"};
}  // namespace

cmd::Version::Version()
    : cli::CommandWithFlagsAndAction{"version", "prints version information", "prints version information"} {
  action([this](const cli::Command::Context& ctxt) {
    util::FormattingLogger log{create_default_logger(ctxt.cout)};
    log.infof(component, "airmap version %d.%d.%d", ::airmap::Version::current().major,
              ::airmap::Version::current().minor, ::airmap::Version::current().patch);
    return 0;
  });
}
