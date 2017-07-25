#include <airmap/cmds/airmap/cmd/simulate_telemetry.h>

#include <airmap/client.h>
#include <airmap/util/telemetry_simulator.h>

#include <thread>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {
auto polygon = airmap::Geometry::polygon(
    {airmap::Geometry::Coordinate{47.37708083985247, 8.546290397644043, airmap::Optional<double>{},
                                  airmap::Optional<double>{}},
     airmap::Geometry::Coordinate{47.37629618703235, 8.545989990234375, airmap::Optional<double>{},
                                  airmap::Optional<double>{}},
     airmap::Geometry::Coordinate{47.37667398429217, 8.548049926757812, airmap::Optional<double>{},
                                  airmap::Optional<double>{}},
     airmap::Geometry::Coordinate{47.37708083985247, 8.546290397644043, airmap::Optional<double>{},
                                  airmap::Optional<double>{}}});
}

cmd::SimulateTelemetry::SimulateTelemetry()
    : cli::CommandWithFlagsAndAction{
          cli::Name{"simulate-telemetry"},
          cli::Usage{"inject artificial telemetry data for a given flight"},
          cli::Description{"inject artificial telemetry data for a given flight"}} {
  flag(cli::make_flag(cli::Name{"api-key"},
                      cli::Description{"api-key for authenticating with the AirMap services"},
                      params_.api_key));
  flag(cli::make_flag(cli::Name{"authorization"},
                      cli::Description{"token used for authorizing with the AirMap services"},
                      params_.authorization));
  flag(cli::make_flag(cli::Name{"host"}, cli::Description{"telemetry host address"}, params_.host));
  flag(cli::make_flag(cli::Name{"port"}, cli::Description{"telemetry host port"}, params_.port));
  flag(cli::make_flag(cli::Name{"frequency"},
                      cli::Description{"telemetry is sent with `FREQUENCY` Hz"},
                      params_.frequency));
  flag(cli::make_flag(cli::Name{"flight-id"},
                      cli::Description{"telemetry is sent for this flight id"}, params_.flight.id));

  action([this](const cli::Command::Context& ctxt) {
    ctxt.cout << "Sending telemetry package to" << std::endl
              << "  host:      " << params_.host << std::endl
              << "  port:      " << params_.port << std::endl
              << "  frequency: " << params_.frequency << std::endl
              << "  flight-id: " << params_.flight.id << std::endl
              << "  api-key:   " << params_.api_key << std::endl
              << "  enc-key:   " << params_.encryption_key << std::endl;

    ::setenv("AIRMAP_TELEMETRY_HOST", params_.host.c_str(), 1);
    ::setenv("AIRMAP_TELEMETRY_PORT", boost::lexical_cast<std::string>(params_.port).c_str(), 1);

    auto result = Client::create_with_credentials(
        Client::Credentials{params_.api_key}, [this, &ctxt](const Client::CreateResult& result) {
          if (not result)
            return;

          auto client = result.value();

          client->flights().start_flight_communications(
              Flights::StartFlightCommunications::Parameters{params_.authorization,
                                                             params_.flight.id},
              [this, &ctxt, client](const Flights::StartFlightCommunications::Result& result) {
                if (!result) {
                  ctxt.cout << "Failed to start flight communications" << std::endl;
                  return;
                }
                std::thread submitter{[this, &ctxt, client, result]() {
                  util::TelemetrySimulator simulator{polygon.details_for_polygon()};

                  while (true) {
                    auto data = simulator.update();

                    client->telemetry().submit_updates(
                        params_.flight, result.value().key,
                        {Telemetry::Update{
                            Telemetry::Position{milliseconds_since_epoch(Clock::universal_time()),
                                                data.latitude, data.longitude, 100, 100, 2}}});
                    std::this_thread::sleep_for(
                        std::chrono::milliseconds{1000 / params_.frequency});
                  }
                }};

                submitter.detach();
              });
        });

    if (result)
      result.value()->run();

    return 0;
  });
}