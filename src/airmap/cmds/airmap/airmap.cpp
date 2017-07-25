#include <airmap/aircrafts.h>
#include <airmap/airspaces.h>
#include <airmap/authenticator.h>
#include <airmap/client.h>
#include <airmap/flights.h>
#include <airmap/telemetry.h>

#include <airmap/util/telemetry_simulator.h>

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

namespace {

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

// constexpr const char* api_key{"E8D05ADD-DF71-3D14-3794-93FAF8ED8F59"};
constexpr const char* api_key{
    "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9."
    "eyJjcmVkZW50aWFsX2lkIjoiY3JlZGVudGlhbHxrOEFkMmxtU29PWXlHR1NlcWdMMGJpTzNvbW1tIiwiYXBwbGljYXRpb2"
    "5faWQiOiJhcHBsaWNhdG"
    "lvbnxENkc0V2JsSU1wR1piZXVweHdENTRoTUFiblE1Iiwib3JnYW5pemF0aW9uX2lkIjoiZGV2ZWxvcGVyfDN4OWQ1YThV"
    "V21iRGJYUzZkNlJKYkhQ"
    "THlaUHkiLCJpYXQiOjE1MDAzMDg2NzJ9.szMZec56G-HjeWT5XfG9vsgJUHwgnRcvzMx4nwPpkTg"};

auto polygon = airmap::Geometry::polygon(
    {airmap::Geometry::Coordinate{47.37708083985247, 8.546290397644043, airmap::Optional<double>{},
                                  airmap::Optional<double>{}},
     airmap::Geometry::Coordinate{47.37629618703235, 8.545989990234375, airmap::Optional<double>{},
                                  airmap::Optional<double>{}},
     airmap::Geometry::Coordinate{47.37667398429217, 8.548049926757812, airmap::Optional<double>{},
                                  airmap::Optional<double>{}},
     airmap::Geometry::Coordinate{47.37708083985247, 8.546290397644043, airmap::Optional<double>{},
                                  airmap::Optional<double>{}}});

}  // namespace

int main() {
  Barrier barrier;

  std::shared_ptr<airmap::Client> client;

  airmap::Client::create_with_credentials(
      airmap::Client::Credentials{api_key}, [&client](const airmap::Client::CreateResult& result) {
    if (not result)
      return;

    client = result.value();

    client->authenticator().authenticate_anonymously(
        airmap::Authenticator::AuthenticateAnonymously::Params{"thomas@airmap.com"},
        [client](const airmap::Authenticator::AuthenticateAnonymously::Result& result) {
          std::cout << "Finished authenticating with the AirMap services "
                    << (result.has_error() ? "with error." : "successfully.") << std::endl;
          if (result) {
            std::cout << "  id: " << result.value().id << std::endl;
            airmap::Flights::CreateFlight::Parameters params;
            params.latitude      = 47.37629618703235;
            params.longitude     = 8.545989990234375;
            params.aircraft_id   = "aircraft|K7yGZ0LT5DEymAH4X4pL2uJE4B9X";
            params.authorization = result.value().id;
            params.start_time    = airmap::Clock::universal_time();
            params.end_time      = airmap::Clock::universal_time() + airmap::Minutes{5};
            params.geometry      = polygon;
            client->flights().create_flight_by_polygon(
                params, [ auth = result.value().id,
                          client ](const airmap::Flights::CreateFlight::Result& result) {
                  std::cout << "Finished creating flight by polygon "
                            << (result.has_error() ? "with error." : "successfully.") << std::endl;
                  if (result) {
                    std::cout << "  " << result.value().id << std::endl;
                        client->flights().start_flight_communications(
                            airmap::Flights::StartFlightCommunications::Parameters{auth, result.value().id}, [
                              client, flight = result.value()
                            ](const airmap::Flights::StartFlightCommunications::Result& result) {
              std::cout << "Finished starting flight comms "
                        << (result.has_error() ? "with error." : "successfully.") << std::endl;
              if (result) {
                std::cout << "  key: " << result.value().key << std::endl;
                std::thread submitter{[client, flight, key = result.value().key](){
                    airmap::util::TelemetrySimulator simulator{polygon.details_for_polygon()};
                while (true) {
                  auto position = simulator.update();
                  client->telemetry().submit_updates(
                      flight, key,
                      {airmap::Telemetry::Update{airmap::Telemetry::Position{
                          airmap::milliseconds_since_epoch(airmap::Clock::universal_time()),
                          position.latitude, position.longitude, 100, 100, 2}}});
                  std::this_thread::sleep_for(std::chrono::milliseconds{200});
                }
              }};
                                submitter.detach();
                  }
                });
          }
        });

    params.geometry = polygon;

    /*client->flights().create_flight_by_polygon(
        params, [ auth = result.value().id, client ](const airmap::Flights::CreateFlight::Result&
       result) { std::cout << "Finished creating flight by polygon "
                    << (result.has_error() ? "with error." : "successfully.") << std::endl;
          if (result) {
            std::cout << "  " << result.value().id << std::endl;
            client->flights().start_flight_communications(
                airmap::Flights::StartFlightCommunications::Parameters{auth, result.value().id},
                [](const airmap::Flights::StartFlightCommunications::Result& result) {
                  std::cout << "Finished starting flight comms "
                            << (result.has_error() ? "with error." : "successfully.") << std::endl;
                  if (result) std::cout << "  key: " << result.value().key << std::endl;
                });
            client->flights().end_flight(airmap::Flights::EndFlight::Parameters{auth,
       result.value().id},
                                         [](const airmap::Flights::EndFlight::Result& result) {
                                           std::cout
                                               << "Finished ending flight "
                                               << (result.has_error() ? "with error." :
       "successfully.")
                                               << std::endl;
                                         });
          }
        });*/
              }
});

/*client->aircrafts().models(
    airmap::Aircrafts::Models::Parameters{}, [client](const airmap::Aircrafts::Models::Result&
result) { std::cout << "Finished querying for aircraft models "
                << (result.has_error() ? "with error." : "successfully.") << std::endl;
      if (result.has_value()) {
        for (const auto& aircraft : result.value()) {
          std::cout << "  " << aircraft.id << " " << aircraft.name << " " <<
aircraft.manufacturer.id << " "
                    << aircraft.manufacturer.name << std::endl;
          client->aircrafts().model_for_id(airmap::Aircrafts::ModelForId::Parameters{aircraft.id},
                                           [client](const airmap::Aircrafts::ModelForId::Result&
result) { std::cout << "Finished querying for aircraft model "
                                                       << (result.has_error() ? "with error." :
"successfully.")
                                                       << std::endl;
                                             if (result.has_value()) {
                                               std::cout << "  " << result.value().id << " "
                                                         << result.value().name << " "
                                                         << result.value().manufacturer.id << " "
                                                         << result.value().manufacturer.name <<
std::endl;
                                             }
                                           });
        }
      }
    });

client->aircrafts().manufacturers(
    airmap::Aircrafts::Manufacturers::Parameters{}, [](const
airmap::Aircrafts::Manufacturers::Result& result) { std::cout << "Finished querying for aircraft
manufacturers "
                << (result.has_error() ? "with error." : "successfully.") << std::endl;
      if (result.has_value()) {
        for (const auto& manufacturer : result.value())
          std::cout << "  " << manufacturer.id << " " << manufacturer.name << std::endl;
      }
    });

// See http://bl.ocks.org/d/83b15c68f50ef48b95f3bf6efb3391b2
airmap::Airspaces::Search::Parameters params;
params.geometry = polygon;

client->airspaces().search(params, [client](const airmap::Airspaces::Search::Result& result) {
  std::cout << "Finished querying for airspaces " << (result.has_error() ? "with error." :
"successfully.")
            << std::endl;
  if (result.has_value()) {
    for (const auto& airspace : result.value()) {
      std::cout << "  " << airspace.id() << " " << airspace.name() << " " << airspace.city() << " "
                << airspace.type() << std::endl;

      client->airspaces().for_ids({airspace.id()}, [client](const airmap::Airspaces::ForIds::Result&
result) { std::cout << "Finished querying airspaces for ids "
                  << (result.has_error() ? "with error." : "successfully.") << std::endl;
        if (result.has_value())
          for (const auto& airspace : result.value())
            std::cout << "  " << airspace.id() << " " << airspace.name() << " " << airspace.city()
<< " "
                      << airspace.type() << " " << airspace.last_updated() << std::endl;
      });
    }
  } else {
    try {
      std::rethrow_exception(result.error());
    } catch (const std::exception& e) {
      std::cout << "  " << e.what() << std::endl;
    }
  }
});

airmap::Flights::Search::Parameters fsp;
client->flights().search(fsp, [client](const airmap::Flights::Search::Result& result) {
  std::cout << "Finished flights query " << (result.has_error() ? "with error." : "successfully.")
<< std::endl; if (result) { for (const auto& flight : result.value()) { std::cout << "  " <<
flight.id << " " << flight.pilot.id << " " << flight.aircraft.id << " "
                << flight.created_at << " " << flight.start_time << " " << flight.end_time <<
std::endl; airmap::Flights::ForId::Parameters fip; fip.id = flight.id; fip.enhance = true;

      client->flights().for_id(fip, [client](const airmap::Flights::ForId::Result& result) {
        std::cout << "Finished flight query " << (result.has_error() ? "with error." :
"successfully.")
                  << std::endl;
        if (result) {
          const auto& flight = result.value();
          std::cout << "  " << flight.id << " " << flight.pilot.id << " " << flight.aircraft.id << "
"
                    << flight.created_at << " " << flight.start_time << " " << flight.end_time <<
std::endl;
        }
      });
    }
  }
});*/
/*client->flights().create_flight(airmap::Flights::CreateFlightByPoint::Parameters{},
                                [client](const airmap::Flights::CreateFlightByPoint::Result& result)
{ std::cout << "Finished flight creation " << (result.has_error() ? "with error." : "successfully.")
<< std::endl;

    if (result) {
        client->flights().start_flight_communications(
                    airmap::Flights::StartFlightCommunications::Parameters{},
                    [client, flight = result.value()](const
airmap::Flights::StartFlightCommunications::Result& result) { if (result) {
                client->telemetry().submit_updates(flight,
                {
                    airmap::Telemetry::Update{airmap::Telemetry::Position{}},
                    airmap::Telemetry::Update{airmap::Telemetry::Speed{}}
                });
            }
        });
    }
});*/
});

barrier.wait();

return 0;
}
