#include <airmap/cmds/airmap/cmd/create_flight.h>

#include <airmap/client.h>
#include <airmap/codec.h>
#include <airmap/context.h>
#include <airmap/date_time.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

using json = nlohmann::json;

namespace {

void print_flight(std::ostream& out, const airmap::Flight& flight) {
  out << "Created flight with: " << std::endl
      << "  id:         " << flight.id << std::endl
      << "  pilot:      " << flight.pilot.id << std::endl
      << "  aircraft:   " << flight.aircraft.id << std::endl
      << "  latitude:   " << flight.latitude << std::endl
      << "  longitude:  " << flight.longitude << std::endl
      << "  created-at: " << airmap::iso8601::generate(flight.created_at) << std::endl
      << "  start-time: " << airmap::iso8601::generate(flight.start_time) << std::endl
      << "  end-time:   " << airmap::iso8601::generate(flight.end_time) << std::endl;
}

constexpr const char* component{"create-flight"};
}  // namespace

cmd::CreateFlight::CreateFlight()
    : cli::CommandWithFlagsAndAction{cli::Name{"create-flight"},
                                     cli::Usage{"creates a flight and registers it with the airmap services"},
                                     cli::Description{"creates a flight and registers it with the airmap services"}} {
  params_.start_time = Clock::universal_time();
  params_.end_time   = params_.start_time + Minutes(5);

  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      api_key_));
  flag(cli::make_flag(cli::Name{"authorization"},
                      cli::Description{"token used for authorizing with the AirMap services"}, authorization_));
  flag(cli::make_flag(cli::Name{"latitude"}, cli::Description{"latitude of take-off point"}, params_.latitude));
  flag(cli::make_flag(cli::Name{"longitude"}, cli::Description{"longitude of take-off point"}, params_.longitude));
  flag(cli::make_flag(cli::Name{"max-altitude"}, cli::Description{"maximum altitude reached during flight"},
                      params_.max_altitude));
  flag(cli::make_flag(cli::Name{"aircraft-id"}, cli::Description{"id of aircraft that executes the flight"},
                      params_.aircraft_id));
  flag(cli::make_flag(cli::Name{"start-time"}, cli::Description{"planned start-time of flight"}, params_.start_time));
  flag(cli::make_flag(cli::Name{"end-time"}, cli::Description{"planned end-time of flight"}, params_.end_time));
  flag(cli::make_flag(cli::Name{"is-public"}, cli::Description{"mark the flight as publicly visible on dashboards"},
                      params_.is_public));
  flag(cli::make_flag(cli::Name{"give-digital-notice"}, cli::Description{"give digital notice to regulators"},
                      params_.give_digital_notice));
  flag(cli::make_flag(cli::Name{"buffer"}, cli::Description{"radius of flight zone centered around the take-off point"},
                      params_.buffer));
  flag(cli::make_flag(cli::Name{"geometry-file"}, cli::Description{"use the polygon defined in this geojson file"},
                      geometry_file_));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger(create_default_logger(ctxt.cout));

    if (!api_key_) {
      log_.errorf(component, "missing parameter 'api-key'");
      return 1;
    }

    if (!api_key_.get().validate()) {
      log_.errorf(component, "parameter 'api-key' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!authorization_) {
      log_.errorf(component, "missing parameter 'authorization'");
      return 1;
    }

    if (!authorization_.get().validate()) {
      log_.errorf(component, "parameter 'authorization' for accessing AirMap services must not be empty");
      return 1;
    }

    params_.authorization = authorization_.get();

    if (!geometry_file_) {
      log_.errorf(component, "missing parameter 'geometry-file'");
      return 1;
    }

    if (geometry_file_) {
      std::ifstream in{geometry_file_.get()};
      if (!in) {
        log_.errorf(component, "failed to open %s for reading", geometry_file_.get());
        return 1;
      }
      Geometry geometry = json::parse(in);
      params_.geometry  = geometry;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      log_.errorf(component, "Could not acquire resources for accessing AirMap services");
      return 1;
    }

    auto context = result.value();

    context->create_client_with_credentials(
        Client::Credentials{api_key_.get()},
        [this, &ctxt, context](const ::airmap::Context::ClientCreateResult& result) {
          if (not result)
            return;

          auto client = result.value();

          auto handler = [this, &ctxt, context, client](const Flights::CreateFlight::Result& result) {
            if (result)
              print_flight(ctxt.cout, result.value());
            else
              log_.errorf(component, "Failed to create flight");
            context->stop();
          };

          if (!params_.geometry)
            client->flights().create_flight_by_point(params_, handler);
          else
            client->flights().create_flight_by_polygon(params_, handler);
        });

    context->run();

    return 0;
  });
}
