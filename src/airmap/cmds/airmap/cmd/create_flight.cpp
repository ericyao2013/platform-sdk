#include <airmap/cmds/airmap/cmd/create_flight.h>

#include <airmap/client.h>
#include <airmap/date_time.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

cmd::CreateFlight::CreateFlight()
    : cli::CommandWithFlagsAndAction{
          cli::Name{"create-flight"},
          cli::Usage{"creates a flight and registers it with the airmap services"},
          cli::Description{"creates a flight and registers it with the airmap services"}} {
  params_.start_time = Clock::universal_time();
  params_.end_time   = params_.start_time + Minutes(5);

  flag(cli::make_flag(cli::Name{"api-key"},
                      cli::Description{"api-key for authenticating with the AirMap services"},
                      api_key_));
  flag(cli::make_flag(cli::Name{"authorization"},
                      cli::Description{"token used for authorizing with the AirMap services"},
                      params_.authorization));
  flag(cli::make_flag(cli::Name{"latitude"}, cli::Description{"latitude of take-off point"},
                      params_.latitude));
  flag(cli::make_flag(cli::Name{"longitude"}, cli::Description{"longitude of take-off point"},
                      params_.longitude));
  flag(cli::make_flag(cli::Name{"max-altitude"},
                      cli::Description{"maximum altitude reached during flight"},
                      params_.max_altitude));
  flag(cli::make_flag(cli::Name{"aircraft-id"},
                      cli::Description{"id of aircraft that executes the flight"},
                      params_.aircraft_id));
  flag(cli::make_flag(cli::Name{"start-time"}, cli::Description{"planned start-time of flight"},
                      params_.start_time));
  flag(cli::make_flag(cli::Name{"end-time"}, cli::Description{"planned end-time of flight"},
                      params_.end_time));
  flag(cli::make_flag(cli::Name{"is-public"},
                      cli::Description{"mark the flight as publicly visible on dashboards"},
                      params_.is_public));
  flag(cli::make_flag(cli::Name{"give-digital-notice"},
                      cli::Description{"give digital notice to regulators"},
                      params_.give_digital_notice));
  flag(cli::make_flag(cli::Name{"buffer"},
                      cli::Description{"radius of flight zone centered around the take-off point"},
                      params_.buffer));

  action([this](const cli::Command::Context& ctxt) {
    auto result = Client::create_with_credentials(
        Client::Credentials{api_key_}, [this, &ctxt](const Client::CreateResult& result) {
          if (not result)
            return;

          auto client = result.value();

          client->flights().create_flight_by_point(
              params_, [this, &ctxt, client](const Flights::CreateFlight::Result& result) {
                if (result) {
                  ctxt.cout << "Created flight with: " << std::endl
                            << "  id: " << result.value().id << std::endl
                            << "  pilot: " << result.value().pilot.id << std::endl
                            << "  aircraft: " << result.value().aircraft.id << std::endl
                            << "  latitude: " << result.value().latitude << std::endl
                            << "  longitude: " << result.value().longitude << std::endl
                            << "  created-at: " << iso8601::generate(result.value().created_at)
                            << std::endl
                            << "  start-time: " << iso8601::generate(result.value().start_time)
                            << std::endl
                            << "  end-time: " << iso8601::generate(result.value().end_time)
                            << std::endl;
                } else {
                  ctxt.cout << "Failed to create flight with id" << std::endl;
                }
              });
        });

    if (result)
      result.value()->run();

    return 0;
  });
}