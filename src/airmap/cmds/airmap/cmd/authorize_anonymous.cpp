#include <airmap/cmds/airmap/cmd/authorize_anonymous.h>

#include <airmap/client.h>
#include <airmap/context.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {
constexpr const char* component{"authorize-anonymously"};
}

cmd::AuthorizeAnonymous::AuthorizeAnonymous()
    : cli::CommandWithFlagsAndAction{cli::Name{"authorize-anonymously"},
                                     cli::Usage{"anonymously authorize with the AirMap services"},
                                     cli::Description{"anonymously authorize with the AirMap services"}} {
  flag(cli::make_flag(cli::Name{"version"}, cli::Description{"work against this version of the AirMap services"},
                      params_.version));
  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      params_.api_key));
  flag(cli::make_flag(cli::Name{"user-id"},
                      cli::Description{"user-id used for authorizing anonymously with the AirMap services"},
                      params_.user_id));

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

    if (!params_.user_id) {
      log_.errorf(component, "missing parameter 'user-id'");
      return 1;
    }

    if (!params_.user_id.get().validate()) {
      log_.errorf(component, "parameter 'user-id' for accessing AirMap services must not be empty");
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

          auto handler = [this, &ctxt, context, client](const Authenticator::AuthenticateAnonymously::Result& result) {
            if (result)
              ctxt.cout << "Authenticated successfully and received id: " << result.value().id << std::endl;
            else
              ctxt.cout << "Failed to authorize " << params_.user_id.get() << std::endl;

            context->stop();
          };

          client->authenticator().authenticate_anonymously(
              Authenticator::AuthenticateAnonymously::Params{params_.user_id.get()}, handler);
        });

    if (result)
      result.value()->run();

    return 0;
  });
}
