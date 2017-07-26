#include <airmap/cmds/airmap/cmd/authorize.h>

#include <airmap/client.h>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;

cmd::Authorize::Authorize()
    : cli::CommandWithFlagsAndAction{cli::Name{"authorize"}, cli::Usage{"authorize with the AirMap services"},
                                     cli::Description{"authorize with the AirMap services"}} {
  flag(cli::make_flag(cli::Name{"api-key"}, cli::Description{"api-key for authenticating with the AirMap services"},
                      api_key_));
  flag(cli::make_flag(cli::Name{"user-id"}, cli::Description{"user-id used for authorizing with the AirMap services"},
                      params_.user_id));

  action([this](const cli::Command::Context& ctxt) {
    auto result = Client::create_with_credentials(
        Client::Credentials{api_key_}, [this, &ctxt](const Client::CreateResult& result) {
          if (not result)
            return;

          auto context = result.value().context;
          auto client  = result.value().client;

          auto handler = [this, &ctxt, context, client](const Authenticator::AuthenticateAnonymously::Result& result) {
            if (result)
              ctxt.cout << "Authenticated successfully and received id: " << result.value().id << std::endl;
            else
              ctxt.cout << "Failed to authorize " << params_.user_id << std::endl;

            context->stop();
          };

          client->authenticator().authenticate_anonymously(params_, handler);
        });

    if (result)
      result.value()->run();

    return 0;
  });
}