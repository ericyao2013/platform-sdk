#ifndef AIRMAP_PILOT_H_
#define AIRMAP_PILOT_H_

#include <airmap/optional.h>

#include <string>

namespace airmap {

struct Pilot {
  std::string id;
  std::string username;
  Optional<std::string> image;
};

}  // namespace airmap

#endif  // AIRMAP_PILOT_H_
