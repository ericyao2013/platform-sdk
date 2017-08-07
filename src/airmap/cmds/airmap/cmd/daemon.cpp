#include <airmap/cmds/airmap/cmd/daemon.h>

#include <airmap/daemon.h>
#include <airmap/mavlink/boost/serial_channel.h>
#include <airmap/rest/boost/communicator.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {
constexpr const char* component{"daemon"};
}

cmd::Daemon::Daemon()
    : cli::CommandWithFlagsAndAction{cli::Name{"daemon"}, cli::Usage{"runs the airmap daemon"},
                                     cli::Description{"runs the airmap daemon"}} {
  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      api_key_));
  flag(cli::make_flag(cli::Name{"user-id"}, cli::Description{"user-id for authenticating with the AirMap services"},
                      user_id_));
  flag(cli::make_flag(cli::Name{"aircraft-id"}, cli::Description{"id of the device the daemon runs on"}, aircraft_id_));
  flag(cli::make_flag(cli::Name{"serial-device"}, cli::Description{"the device file to read mavlink messages from"},
                      serial_device_));

  action([this](const cli::Command::Context& ctxt) {
    log_ = util::FormattingLogger{create_default_logger(ctxt.cout)};

    if (!api_key_) {
      log_.errorf(component, "missing parameter 'api-key'");
      return 1;
    }

    if (!api_key_.get().validate()) {
      log_.errorf(component, "parameter 'api-key' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!user_id_) {
      log_.errorf(component, "missing parameter 'user-id'");
      return 1;
    }

    if (!user_id_.get().validate()) {
      log_.errorf(component, "parameter 'user-id' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!aircraft_id_) {
      log_.errorf(component, "missing parameter 'aircraft-id'");
      return 1;
    }

    if (!aircraft_id_.get().validate()) {
      log_.errorf(component, "parameter 'aircraft-id' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!serial_device_) {
      log_.errorf(component, "missing parameter 'serial-device'");
      return 1;
    }

    auto communicator = std::make_shared<rest::boost::Communicator>(log_.logger());

    auto channel = std::make_shared<mavlink::boost::SerialChannel>(log_.logger(), communicator->io_service(),
                                                                   serial_device_.get());

    communicator->create_client_with_credentials(
        Client::Credentials{api_key_.get()},
        [this, communicator, channel](const ::airmap::Context::ClientCreateResult& result) {
          if (not result)
            return;

          ::airmap::Daemon::Configuration configuration{api_key_.get(), user_id_.get(), aircraft_id_.get(),
                                                        log_.logger(),   channel,  result.value()};

          std::make_shared<::airmap::Daemon>(configuration)->start();
        });

    communicator->run();

    return 0;
  });
}
