#include <airmap/cmds/airmap/cmd/start_flight_comms.h>

#include <airmap/client.h>
#include <airmap/codec.h>
#include <airmap/context.h>
#include <airmap/util/telemetry_simulator.h>

#include <fstream>
#include <iterator>
#include <thread>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

using json = nlohmann::json;

namespace {
constexpr const char* component{"start-flight-comms"};
}

cmd::StartFlightComms::StartFlightComms()
    : cli::CommandWithFlagsAndAction{cli::Name{"start-flight-comms"},
                                     cli::Usage{"prepare flight for injection of telemetry data"},
                                     cli::Description{"prepare flight for injection of telemetry data"}} {
  flag(cli::make_flag(cli::Name{"version"}, cli::Description{"work against this version of the AirMap services"},
                      params_.version));
  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      params_.api_key));
  flag(cli::make_flag(cli::Name{"authorization"},
                      cli::Description{"token used for authorizing with the AirMap services"}, params_.authorization));
  flag(cli::make_flag(cli::Name{"flight-id"}, cli::Description{"telemetry is sent for this flight id"},
                      params_.flight_id));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_default_logger()};

    if (!params_.api_key) {
      log_.errorf(component, "missing parameter 'api-key'");
      return 1;
    }

    if (!params_.api_key.get().validate()) {
      log_.errorf(component, "parameter 'api-key' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!params_.authorization) {
      log_.errorf(component, "missing parameter 'authorization'");
      return 1;
    }

    if (!params_.authorization.get().validate()) {
      log_.errorf(component, "parameter 'authorization' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!params_.flight_id) {
      log_.errorf(component, "missing parameter 'flight-id'");
      return 1;
    }

    if (!params_.flight_id.get().validate()) {
      log_.errorf(component, "parameter 'flight-id' for accessing AirMap services must not be empty");
      return 1;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      ctxt.cout << "Could not acquire resources for accessing AirMap services" << std::endl;
      return 1;
    }

    auto context = result.value();
    auto config  = Client::default_configuration(params_.version, Client::Credentials{params_.api_key.get()});

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
          if (not result)
            return;

          auto client = result.value();

          client->flights().start_flight_communications(
              Flights::StartFlightCommunications::Parameters{params_.authorization.get(), params_.flight_id.get()},
              [this, &ctxt, context, client](const Flights::StartFlightCommunications::Result& result) {
                if (!result) {
                  ctxt.cout << "Failed to start flight communications" << std::endl;
                  context->stop();
                  return;
                }

                ctxt.cout << "Started flight communications: " << result.value().key << std::endl;
                context->stop();
              });
        });

    context->run();
    return 0;
  });
}
