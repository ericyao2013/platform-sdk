#ifndef AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_ANONYMOUS_H_
#define AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_ANONYMOUS_H_

#include <airmap/authenticator.h>
#include <airmap/util/cli.h>
#include <airmap/util/formatting_logger.h>
#include <airmap/util/tagged_string.h>

namespace airmap {
namespace cmds {
namespace airmap {
namespace cmd {

class AuthorizeAnonymous : public util::cli::CommandWithFlagsAndAction {
 public:
  AuthorizeAnonymous();

 private:
  using ApiKey = util::TaggedString<util::tags::MustNotBeEmpty>;
  using UserId = util::TaggedString<util::tags::MustNotBeEmpty>;

  util::FormattingLogger log_{create_null_logger()};
  struct {
    Optional<ApiKey> api_key;
    Optional<UserId> user_id;
  } params_;
};

}  // namespace cmd
}  // namespace airmap
}  // namespace cmds
}  // namespace airmap

#endif  // AIRMAP_CMDS_AIRMAP_CMD_AUTHORIZE_ANONYMOUS_H_
