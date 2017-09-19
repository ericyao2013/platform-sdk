#ifndef AIRMAP_VERSION_H_
#define AIRMAP_VERSION_H_

#include <airmap/date_time.h>
#include <airmap/optional.h>

#include <cstdint>

namespace airmap {

struct Version {
  static const Version& current();

  std::uint32_t major;
  std::uint32_t minor;
  std::uint32_t patch;
  Optional<std::string> git_revision;
  Optional<DateTime> build_timestamp;
};

}  // namespace airmap

#endif  // AIRMAP_VERSION_H_
