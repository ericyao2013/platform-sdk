#ifndef AIRMAP_REST_AUTHENTICATOR_H_
#define AIRMAP_REST_AUTHENTICATOR_H_

#include <airmap/authenticator.h>

#include <airmap/rest/communicator.h>

namespace airmap {
namespace rest {

class Authenticator : public airmap::Authenticator {
 public:
  explicit Authenticator(const std::string& host, Client::Version version, Communicator& communicator);

  void authenticate_with_password(const AuthenticateWithPassword::Params& params,
                                  const AuthenticateWithPassword::Callback& cb) override;

  void authenticate_anonymously(const AuthenticateAnonymously::Params& params,
                                const AuthenticateAnonymously::Callback& cb) override;

  void renew_authentication(const RenewAuthentication::Params& params,
                            const RenewAuthentication::Callback& cb) override;

 private:
  std::string host_;
  Client::Version version_;
  Communicator& communicator_;
};

}  // namespace rest
}  // namespace airmap

#endif  // AIRMAP_REST_AUTHENTICATOR_H_
