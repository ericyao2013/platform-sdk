#include <airmap/cmds/airmap/cmd/authorize_password.h>

#include <airmap/client.h>
#include <airmap/context.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {
constexpr const char* component{"authorize-password"};
}

cmd::AuthorizePassword::AuthorizePassword()
    : cli::CommandWithFlagsAndAction{cli::Name{"authorize-password"}, cli::Usage{"authorize with the AirMap services"},
                                     cli::Description{"authorize with the AirMap services"}} {
  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      params_.api_key));
  flag(cli::make_flag(cli::Name{"client-id"},
                      cli::Description{"client-id used for authorizing with the AirMap services"}, params_.client_id));
  flag(cli::make_flag(cli::Name{"username"}, cli::Description{"username used for authorizing with the AirMap services"},
                      params_.username));
  flag(cli::make_flag(cli::Name{"password"}, cli::Description{"password used for authorizing with the AirMap services"},
                      params_.password));
  flag(cli::make_flag(cli::Name{"device"}, cli::Description{"device used for authorizing with the AirMap services"},
                      params_.device));

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

    if (!params_.client_id) {
      log_.errorf(component, "missing parameter 'client-id'");
      return 1;
    }

    if (!params_.client_id.get().validate()) {
      log_.errorf(component, "parameter 'client-id' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!params_.username) {
      log_.errorf(component, "missing parameter 'username'");
      return 1;
    }

    if (!params_.username.get().validate()) {
      log_.errorf(component, "parameter 'username' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!params_.password) {
      log_.errorf(component, "missing parameter 'password'");
      return 1;
    }

    if (!params_.password.get().validate()) {
      log_.errorf(component, "parameter 'password' for accessing AirMap services must not be empty");
      return 1;
    }

    if (!params_.device) {
      log_.errorf(component, "missing parameter 'device'");
      return 1;
    }

    if (!params_.device.get().validate()) {
      log_.errorf(component, "parameter 'device' for accessing AirMap services must not be empty");
      return 1;
    }

    auto result = ::airmap::Context::create(log_.logger());

    if (!result) {
      ctxt.cout << "Could not acquire resources for accessing AirMap services" << std::endl;
      return 1;
    }

    auto context = result.value();

    context->create_client_with_credentials(
        Client::Credentials{params_.api_key.get()},
        [this, &ctxt, context](const ::airmap::Context::ClientCreateResult& result) {
          if (not result)
            return;

          auto client = result.value();

          auto handler = [this, &ctxt, context, client](const Authenticator::AuthenticateWithPassword::Result& result) {
            if (result) {
              ctxt.cout << "Authenticated successfully and received id: " << result.value().id << std::endl;
            } else
              ctxt.cout << "Failed to authorize " << params_.username.get() << std::endl;

            context->stop();
          };

          client->authenticator().authenticate_with_password(
              Authenticator::AuthenticateWithPassword::Params{params_.client_id.get(), params_.username.get(),
                                                              params_.password.get(), params_.device.get()},
              handler);
        });

    if (result)
      result.value()->run();

    return 0;
  });
}
