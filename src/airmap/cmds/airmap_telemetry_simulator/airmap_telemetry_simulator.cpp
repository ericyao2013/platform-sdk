#include <airmap/client.h>
#include <airmap/date_time.h>
#include <airmap/flight.h>
#include <airmap/geometry.h>
#include <airmap/optional.h>
#include <airmap/telemetry.h>

#include <airmap/util/cli.h>

#include <iostream>
#include <mutex>
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

class Barrier {
 public:
  void wait() {
    std::unique_lock<std::mutex> ul{mutex_};
    condition_variable_.wait(ul, [this]() { return signaled_; });
  }

  void signal_all() {
    std::unique_lock<std::mutex> ul{mutex_};
    signaled_ = true;
    condition_variable_.notify_all();
  }

 private:
  std::mutex mutex_;
  bool signaled_{false};
  std::condition_variable condition_variable_;
};

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
  airmap::Flight flight;
  std::string api_key;
  std::string encryption_key;
} params;

}  // namespace

int main(int argc, char** argv) {
  cli::CommandWithFlagsAndAction cmd{cli::Name{"airmap-telemetry-simulator"},
                                     cli::Usage{"simulates telemetry submission"},
                                     cli::Description{"simulates telemetry submission"}};

  cmd.flag(
         cli::make_flag(cli::Name{"host"}, cli::Description{"telemetry host address"}, params.host))
      .flag(cli::make_flag(cli::Name{"port"}, cli::Description{"telemetry host port"}, params.port))
      .flag(cli::make_flag(cli::Name{"flight-id"},
                           cli::Description{"telemetry is sent for this flight id"},
                           params.flight.id))
      .flag(cli::make_flag(cli::Name{"api-key"},
                           cli::Description{"key used to authenticate access to AirMap APIs"},
                           params.api_key))
      .flag(cli::make_flag(cli::Name{"enc-key"},
                           cli::Description{"telemetry is encrypted with this key"},
                           params.encryption_key));

  cmd.action([](const cli::Command::Context& ctxt) {
    ctxt.cout << "Sending telemetry package to" << std::endl
              << "  host:      " << params.host << std::endl
              << "  port:      " << params.port << std::endl
              << "  flight-id: " << params.flight.id << std::endl
              << "  api-key:   " << params.api_key << std::endl
              << "  enc-key:   " << params.encryption_key << std::endl;
    std::shared_ptr<airmap::DeviceIdentifier> di;
    std::shared_ptr<airmap::SecretsStore> ss;
    std::shared_ptr<airmap::Client> client;

    airmap::Client::create_with_credentials(
        airmap::Client::Credentials{params.api_key}, ss, di,
        [&client](const airmap::Client::CreateResult& result) {
          if (!result) {
            return;
          }

          client = result.value();

          std::thread submitter{[client]() {
            std::uint32_t counter{0};
            while (true) {
              auto coord = polygon.details_for_polygon().at(0).coordinates.at(
                  counter++ % polygon.details_for_polygon().at(0).coordinates.size());
              client->telemetry().submit_updates(
                  params.flight, params.encryption_key,
                  {airmap::Telemetry::Update{airmap::Telemetry::Position{
                      airmap::milliseconds_since_epoch(airmap::Clock::universal_time()),
                      coord.latitude, coord.longitude, 100, 100, 2}}});
              std::this_thread::sleep_for(std::chrono::milliseconds{200});
            }
          }};

          submitter.detach();
        });

    Barrier barrier;
    barrier.wait();

    return 0;
  });
  return cmd.run(cli::Command::Context{std::cin, std::cout, cli::args(argc, argv)});
}
