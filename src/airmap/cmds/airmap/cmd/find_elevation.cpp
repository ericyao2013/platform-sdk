#include <airmap/cmds/airmap/cmd/find_elevation.h>

#include <airmap/client.h>
#include <airmap/codec.h>
#include <airmap/context.h>
#include <airmap/date_time.h>
#include <airmap/paths.h>

#include <signal.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

using json = nlohmann::json;

namespace {

constexpr const char* component{"find-elevation"};

void print_elevation(std::ostream& out, const std::vector<std::int32_t>& v) {
  cli::TabWriter tw;
  tw << "elevation" << cli::TabWriter::NewLine{};
  for (const auto& e : v)
    tw << e << cli::TabWriter::NewLine{};
  tw.flush(out);
}

}  // namespace

cmd::FindElevation::FindElevation()
    : cli::CommandWithFlagsAndAction{"find-elevation", "finds elevation for given coordinates",
                                     "provides elevation data for all locations on the surface of the earth"} {
  flag(flags::version(version_));
  flag(flags::log_level(log_level_));
  flag(flags::config_file(config_file_));
  flag(cli::make_flag("points", "comma-separated list of coordinates", points_));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_filtering_logger(log_level_, create_default_logger(ctxt.cerr))};

    if (!config_file_) {
      config_file_ = ConfigFile{paths::config_file(version_).string()};
    }

    std::ifstream in_config{config_file_.get()};
    if (!in_config) {
      log_.errorf(component, "failed to open configuration file %s for reading", config_file_);
      return 1;
    }

    if (!points_) {
      log_.errorf(component, "missing parameter 'points'");
      return 1;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "failed to acquire resources for accessing AirMap services");
      return 1;
    }

    auto context = result.value();
    auto config  = Client::load_configuration_from_json(in_config);

    log_.infof(component,
               "client configuration:\n"
               "  host:                %s\n"
               "  version:             %s\n"
               "  telemetry.host:      %s\n"
               "  telemetry.port:      %d\n"
               "  credentials.api_key: %s\n",
               config.host, config.version, config.telemetry.host, config.telemetry.port, config.credentials.api_key);

    context->create_client_with_configuration(
        config, [this, &ctxt, context](const ::airmap::Context::ClientCreateResult& result) {
          if (not result) {
            log_.errorf(component, "failed to create client: %s", result.error());
            context->stop(::airmap::Context::ReturnCode::error);
            return;
          }

          auto client = result.value();

          auto handler = [this, &ctxt, context, client](const Elevation::GetElevationPoints::Result& result) {
            if (result) {
              log_.infof(component, "succesfully obtained elevation for list of coordinates\n");
              print_elevation(ctxt.cout, result.value());
              context->stop();
            } else {
              log_.errorf(component, "failed to obtain elevation: %s", result.error());
              context->stop(::airmap::Context::ReturnCode::error);
              return;
            }
          };

          params_.points = points_.get();
          client->elevation().get_elevation_points(params_, handler);
        });

    return context->exec({SIGINT, SIGQUIT},
                         [this, context](int sig) {
                           log_.infof(component, "received [%s], shutting down", ::strsignal(sig));
                           context->stop();
                         }) == ::airmap::Context::ReturnCode::success
               ? 0
               : 1;
  });
}
