#ifndef AIRMAP_AIRCRAFT_H_
#define AIRMAP_AIRCRAFT_H_

#include <string>

namespace airmap {

struct Aircraft {
  struct Model {
    std::string id;
    std::string name;
  };

  struct Manufacturer {
    std::string id;
    std::string name;
  };

  Model model;
  Manufacturer manufacturer;
};

}  // namespace airmap

#endif  // AIRMAP_AIRCRAFT_H_
