#include <airmap/context.h>

#include <airmap/rest/glib/communicator.h>

airmap::Context::CreateResult airmap::Context::create() {
  return CreateResult{std::make_shared<rest::glib::Communicator>()};
}
