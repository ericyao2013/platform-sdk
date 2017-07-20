#include <airmap/client.h>

#include <airmap/glib/client.h>

void airmap::Client::create_with_credentials(
            const Credentials& credentials,
            const std::shared_ptr<SecretsStore>&,
            const std::shared_ptr<DeviceIdentifier>&,
            const CreateCallback& cb) {
    airmap::glib::Api::create(credentials.api_key, [cb](const airmap::glib::Api::CreateResult& result) {
        using Result = Result<std::shared_ptr<Client>, std::exception_ptr>;
 
        if (result) {
            cb(Result(std::make_shared<glib::Client>(result.value())));
        } else {
            cb(Result(result.error()));
        }
    });
}

airmap::Client::Client() = default;
airmap::Client::~Client() = default;


