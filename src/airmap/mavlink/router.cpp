#include <airmap/mavlink/router.h>

airmap::mavlink::Router::Router(const std::initializer_list<std::shared_ptr<Route>>& routes) : routes_{routes} {
}

void airmap::mavlink::Router::route(const mavlink_message_t& message) {
  for (const auto& route : routes_)
    route->process(message);
}

void airmap::mavlink::Router::start() {
  for (const auto& route : routes_)
    route->start();
}

void airmap::mavlink::Router::stop() {
  for (const auto& route : routes_)
    route->stop();
}
