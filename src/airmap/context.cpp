#include <airmap/context.h>

#include <airmap/rest/glib/communicator.h>

airmap::Context::CreateResult airmap::Context::create(const std::shared_ptr<Logger>& logger) {
  return CreateResult{std::make_shared<rest::glib::Communicator>(logger)};
}
