#include <airmap/platform/null/standard_paths.h>

airmap::Optional<airmap::platform::Path> airmap::platform::null::StandardPaths::path(Scope scope, Location location) {
  return initial_path();
}
