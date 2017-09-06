#include <airmap/net/http/user_agent.h>

#include <airmap/version.h>
#include <fmt/printf.h>

std::string airmap::net::http::user_agent() {
    return fmt::sprintf("AirMap/%d.%d.%d", Version::current().major, Version::current().minor, Version::current().patch);
}
