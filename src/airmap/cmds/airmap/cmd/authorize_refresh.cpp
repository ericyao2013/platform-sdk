#include <airmap/cmds/airmap/cmd/authorize_refresh.h>

#include <airmap/client.h>
#include <airmap/context.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

namespace {
constexpr const char* component{"authorize-refresh"};
}

cmd::AuthorizeRefresh::AuthorizeRefresh()
    : cli::CommandWithFlagsAndAction{cli::Name{"authorize-refresh"},
                                     cli::Usage{"renew authorization with the AirMap services"},
                                     cli::Description{"renew authorization with the AirMap services"}} {
  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      params_.api_key));
  flag(cli::make_flag(cli::Name{"client-id"},
                      cli::Description{"client-id used for authorizing with the AirMap services"}, params_.client_id));
  flag(cli::make_flag(cli::Name{"refresh-token"},
                      cli::Description{"refresh-token used for authorizing with the AirMap services"},
                      params_.refresh_token));

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

    if (!params_.refresh_token) {
      log_.errorf(component, "missing parameter 'refresh-token'");
      return 1;
    }

    if (!params_.refresh_token.get().validate()) {
      log_.errorf(component, "parameter 'refresh-token' for accessing AirMap services must not be empty");
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

          auto handler = [this, &ctxt, context, client](const Authenticator::RenewAuthentication::Result& result) {
            if (result) {
              ctxt.cout << "Refreshed token successfully and received id: "
                        << "  id:         " << result.value().id << std::endl
                        << "  expires in: " << result.value().expires_in.count() << std::endl;
            } else
              ctxt.cout << "Failed to renew with " << params_.refresh_token.get() << std::endl;

            context->stop();
          };

          client->authenticator().renew_authentication(
              Authenticator::RenewAuthentication::Params{params_.client_id.get(), params_.refresh_token.get()},
              handler);

        });

    if (result)
      result.value()->run();

    return 0;
  });
}
