#ifndef AIRMAP_GLIB_AUTHENTICATOR_H_
#define AIRMAP_GLIB_AUTHENTICATOR_H_

#include <airmap/authenticator.h>

#include <airmap/glib/api.h>

namespace airmap {
namespace glib {

class Authenticator : public airmap::Authenticator {
 public:
  explicit Authenticator(Api& api);

  void authenticate_with_password(const AuthenticateWithPassword::Params& params,
                                  const AuthenticateWithPassword::Callback& cb) override;

  void authenticate_anonymously(const AuthenticateAnonymously::Params& params,
                                const AuthenticateAnonymously::Callback& cb) override;

  void renew_authentication(const RenewAuthentication::Params& params,
                            const RenewAuthentication::Callback& cb) override;

 private:
  Api& api_;
};

}  // namespace glib
}  // namespace airmap

#endif  // AIRMAP_GLIB_AUTHENTICATOR_H_
