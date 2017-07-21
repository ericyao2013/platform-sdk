#include <airmap/date_time.h>
#include <airmap/geometry.h>
#include <airmap/optional.h>
#include <airmap/telemetry.h>

#include <airmap/util/cli.h>

#include <iostream>
#include <thread>

namespace cli = airmap::util::cli;

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

int main(int argc, char** argv) {
  std::string host;
  std::uint16_t port{16060};
  std::string flight_id;
  std::string key;

  cli::CommandWithFlagsAndAction cmd{cli::Name{"airmap-telemetry-simulator"},
                                     cli::Usage{"simulates telemetry submission"},
                                     cli::Description{"simulates telemetry submission"}};

  cmd
    .flag(cli::make_flag(cli::Name{"host"}, cli::Description{"telemetry host address"}, host))
    .flag(cli::make_flag(cli::Name{"port"}, cli::Description{"telemetry host port"}, port))
    .flag(cli::make_flag(cli::Name{"flight-id"}, cli::Description{"telemetry is sent for this flight id"}, flight_id))
    .flag(cli::make_flag(cli::Name{"key"}, cli::Description{"telemetry is encrypted with this key"}, key))
    .action([&host, &port, &flight_id, &key](const cli::Command::Context& ctxt) {
	return 0;
      });

  return cmd.run(cli::Command::Context{std::cin, std::cout, cli::args(argc, argv)});
}
