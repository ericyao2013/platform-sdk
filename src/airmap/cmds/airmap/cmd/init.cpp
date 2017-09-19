#include <airmap/cmds/airmap/cmd/init.h>

#include <airmap/codec.h>
#include <airmap/paths.h>
#include <airmap/version.h>
#include <nlohmann/json.hpp>

#include <fstream>

namespace cli = airmap::util::cli;
namespace cmd = airmap::cmds::airmap::cmd;
using json    = nlohmann::json;

namespace {
constexpr const char* component{"init"};
}  // namespace

cmd::Init::Init()
    : cli::CommandWithFlagsAndAction{"init", "initializes user-specific configuration data",
                                     "initializes user-specific configuration data"} {
  flag(flags::config_file(config_file_));
  flag(flags::version(version_));
  flag(cli::make_flag("credentials-type", "type of credentials in {anonymous, oauth}", credentials_type_));

  action([this](const cli::Command::Context& ctxt) {
    util::FormattingLogger log{create_default_logger(ctxt.cout)};

    if (!config_file_) {
      config_file_ = ConfigFile{paths::config_file(version_).string()};
    }

    std::ofstream config_file{config_file_.get()};
    if (!config_file) {
      log.errorf(component, "failed to open config file %s for writing", config_file_.get());
      return 1;
    }

    Credentials credentials;
    credentials.api_key = "REPLACE_ME";

    switch (credentials_type_) {
      case Credentials::Type::anonymous:
        credentials.anonymous = Credentials::Anonymous{"REPLACE_ME"};
        break;
      case Credentials::Type::oauth:
        credentials.oauth = Credentials::OAuth{"REPLACE_ME", "REPLACE_ME", "REPLACE_ME", "REPLACE_ME"};
        break;
    }

    json j = Client::default_configuration(version_, credentials);
    config_file << j.dump(2);

    log.infof(component, "persisted configuration to %s", config_file_.get());
    log.infof(component, "please edit %s and fill in your credentials", config_file_.get());

    return 0;
  });
}
