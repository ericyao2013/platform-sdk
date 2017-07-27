#include <airmap/client.h>
#include <airmap/context.h>
#include <airmap/date_time.h>
#include <airmap/flight.h>
#include <airmap/geometry.h>
#include <airmap/optional.h>
#include <airmap/telemetry.h>

#include <airmap/util/cli.h>
#include <airmap/util/telemetry_simulator.h>

#include <boost/lexical_cast.hpp>

#include <stdlib.h>

#include <cmath>

#include <chrono>
#include <iostream>
#include <thread>

namespace cli = airmap::util::cli;

namespace {

auto polygon =
    airmap::Geometry::polygon({airmap::Geometry::Coordinate{47.37708083985247, 8.546290397644043,
                                                            airmap::Optional<double>{}, airmap::Optional<double>{}},
                               airmap::Geometry::Coordinate{47.37629618703235, 8.545989990234375,
                                                            airmap::Optional<double>{}, airmap::Optional<double>{}},
                               airmap::Geometry::Coordinate{47.37667398429217, 8.548049926757812,
                                                            airmap::Optional<double>{}, airmap::Optional<double>{}},
                               airmap::Geometry::Coordinate{47.37708083985247, 8.546290397644043,
                                                            airmap::Optional<double>{}, airmap::Optional<double>{}}});

struct Params {
  Params()
      : host{"127.0.0.1"},
        port{16060},
        api_key{"not-an-api-key"},
        encryption_key{"MDAwMDExMTEyMjIyMzMzMzQ0NDQ1NTU1NjY2Njc3Nzc="} {
    flight.id = "flight|J4G8KLPhm50WW3cMQ2KpniB5GvAg";
  }
  std::string host;
  std::uint16_t port{16060};
  std::uint16_t frequency{5};
  airmap::Flight flight;
  std::string api_key;
  std::string encryption_key;
} params;

}  // namespace

int main(int argc, char** argv) {
  cli::CommandWithFlagsAndAction cmd{cli::Name{"airmap-telemetry-simulator"},
                                     cli::Usage{"simulates telemetry submission"},
                                     cli::Description{"simulates telemetry submission"}};

  cmd.flag(cli::make_flag(cli::Name{"host"}, cli::Description{"telemetry host address"}, params.host))
      .flag(cli::make_flag(cli::Name{"port"}, cli::Description{"telemetry host port"}, params.port))
      .flag(cli::make_flag(cli::Name{"frequency"}, cli::Description{"telemetry is sent with `FREQUENCY` Hz"},
                           params.frequency))
      .flag(cli::make_flag(cli::Name{"flight-id"}, cli::Description{"telemetry is sent for this flight id"},
                           params.flight.id))
      .flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"key used to authenticate access to AirMap APIs"},
                           params.api_key))
      .flag(cli::make_flag(cli::Name{"enc-key"}, cli::Description{"telemetry is encrypted with this key"},
                           params.encryption_key));

  cmd.action([](const cli::Command::Context& ctxt) {
    auto result = airmap::Context::create(std::shared_ptr<airmap::Logger>{});

    if (!result) {
      ctxt.cout << "Could not acquire resources for accessing AirMap services" << std::endl;
      return 1;
    }

    auto context = result.value();

    ctxt.cout << "Sending telemetry package to" << std::endl
              << "  host:      " << params.host << std::endl
              << "  port:      " << params.port << std::endl
              << "  frequency: " << params.frequency << std::endl
              << "  flight-id: " << params.flight.id << std::endl
              << "  api-key:   " << params.api_key << std::endl
              << "  enc-key:   " << params.encryption_key << std::endl;

    ::setenv("AIRMAP_TELEMETRY_HOST", params.host.c_str(), 1);
    ::setenv("AIRMAP_TELEMETRY_PORT", boost::lexical_cast<std::string>(params.port).c_str(), 1);

    context->create_client_with_credentials(
        airmap::Client::Credentials{params.api_key}, [](const airmap::Context::ClientCreateResult& result) {
          if (!result) {
            return;
          }

          auto client = result.value();

          std::thread submitter{[client]() {
            airmap::util::TelemetrySimulator simulator{polygon.details_for_polygon()};

            while (true) {
              auto data = simulator.update();

              std::cout << data.latitude << ", " << data.longitude << std::endl;
              client->telemetry().submit_updates(params.flight, params.encryption_key,
                                                 {airmap::Telemetry::Update{airmap::Telemetry::Position{
                                                     airmap::milliseconds_since_epoch(airmap::Clock::universal_time()),
                                                     data.latitude, data.longitude, 100, 100, 2}}});
              std::this_thread::sleep_for(std::chrono::milliseconds{1000 / params.frequency});
            }
          }};

          submitter.detach();
        });

    context->run();
    return 0;
  });

  return cmd.run(cli::Command::Context{std::cin, std::cout, cli::args(argc, argv)});
}
