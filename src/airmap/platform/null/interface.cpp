#include <airmap/platform/null/interface.h>

airmap::platform::StandardPaths& airmap::platform::null::Interface::standard_paths() {
  return standard_paths_;
}
