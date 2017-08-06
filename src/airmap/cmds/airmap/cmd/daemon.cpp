#include <airmap/cmds/airmap/cmd/daemon.h>

#include <airmap/daemon.h>
#include <airmap/mavlink/boost/serial_channel.h>
#include <airmap/rest/boost/communicator.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

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
    if (!serial_device_) {
      ctxt.cout << "missing serial device" << std::endl;
      return 1;
    }
    auto logger       = create_default_logger(ctxt.cout);
    auto communicator = std::make_shared<rest::boost::Communicator>(logger);
    auto channel =
        std::make_shared<mavlink::boost::SerialChannel>(logger, communicator->io_service(), serial_device_.get());

    communicator->create_client_with_credentials(
        Client::Credentials{api_key_},
        [this, logger, communicator, channel](const ::airmap::Context::ClientCreateResult& result) {
          if (not result)
            return;

          ::airmap::Daemon::Configuration configuration{api_key_, user_id_, aircraft_id_,
                                                        logger,   channel,  result.value()};

          std::make_shared<::airmap::Daemon>(configuration)->start();
        });

    communicator->run();

    return 0;
  });
}
