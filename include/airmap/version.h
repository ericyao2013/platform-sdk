#ifndef AIRMAP_VERSION_H_
#define AIRMAP_VERSION_H_

#include <cstdint>

namespace airmap {

struct Version {
  static const Version& current();

  std::uint32_t major;
  std::uint32_t minor;
  std::uint32_t patch;
};

}  // namespace airmap

#endif  // AIRMAP_VERSION_H_
