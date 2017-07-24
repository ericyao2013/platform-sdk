#include <airmap/rest/authenticator.h>

#include <airmap/codec.h>
#include <airmap/jsend.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

airmap::rest::Authenticator::Authenticator(Communicator& communicator) : communicator_{communicator} {
}

void airmap::rest::Authenticator::authenticate_with_password(
    const AuthenticateWithPassword::Params&, const AuthenticateWithPassword::Callback&) {
  throw std::runtime_error{"not implemented"};
}

void airmap::rest::Authenticator::authenticate_anonymously(
    const AuthenticateAnonymously::Params& params, const AuthenticateAnonymously::Callback& cb) {
  std::unordered_map<std::string, std::string> headers;

  json j;
  j = params;

  communicator_.post("https://api.airmap.com", "/auth/v1/anonymous/token", std::move(headers), j.dump(),
            [cb](const Communicator::DoResult& result) {
              if (result) {
                auto j = json::parse(result.value());

                if (j[jsend::key::status] == jsend::status::success) {
                  cb(AuthenticateAnonymously::Result(j[jsend::key::data].get<AnonymousToken>()));
                } else {
                  cb(AuthenticateAnonymously::Result(
                      std::make_exception_ptr(std::runtime_error(jsend::stringify_error(j)))));
                }
              }
            });
}

void airmap::rest::Authenticator::renew_authentication(const RenewAuthentication::Params&,
                                                       const RenewAuthentication::Callback&) {
}