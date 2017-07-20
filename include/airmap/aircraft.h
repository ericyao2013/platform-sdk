#ifndef AIRMAP_AIRCRAFT_H_
#define AIRMAP_AIRCRAFT_H_

#include <string>

namespace airmap {

struct Aircraft {
  struct Manufacturer {
    std::string id;
    std::string name;
  };

  std::string id;
  std::string name;
  Manufacturer manufacturer;
};

}  // namespace airmap

#endif  // AIRMAP_AIRCRAFT_H_
