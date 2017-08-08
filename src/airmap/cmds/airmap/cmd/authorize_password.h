#ifndef AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_PASSWORD_H_
#define AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_PASSWORD_H_

#include <airmap/authenticator.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class AuthorizePassword : public util::cli::CommandWithFlagsAndAction {
 public:
  AuthorizePassword();

 private:
  using ApiKey   = util::TaggedString<util::tags::MustNotBeEmpty>;
  using ClientId = util::TaggedString<util::tags::MustNotBeEmpty>;
  using Username = util::TaggedString<util::tags::MustNotBeEmpty>;
  using Password = util::TaggedString<util::tags::MustNotBeEmpty>;
  using Device   = util::TaggedString<util::tags::MustNotBeEmpty>;

  util::FormattingLogger log_{create_null_logger()};
  struct {
    Optional<ApiKey> api_key;
    Optional<ClientId> client_id;
    Optional<Username> username;
    Optional<Password> password;
    Optional<Device> device;
  } params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_PASSWORD_H_
