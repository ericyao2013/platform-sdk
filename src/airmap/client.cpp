#include <airmap/client.h>

#include <airmap/rest/client.h>
#include <airmap/rest/glib/communicator.h>

airmap::Client::ContextResult airmap::Client::create_with_credentials(
    const Credentials& credentials, const CreateCallback& cb) {
  auto result = airmap::rest::glib::Communicator::create(credentials.api_key);

  if (result) {
    auto ctxt = result.value();
    ctxt->dispatch([ctxt, cb]() {
      cb(CreateResult{ClientWithContext{ctxt, std::make_shared<rest::Client>(ctxt)}});
    });
  }
  return ContextResult{std::dynamic_pointer_cast<airmap::Context>(result.value())};
}
