#ifndef AIRMAP_FLIGHT_H_
#define AIRMAP_FLIGHT_H_

#include <airmap/aircraft.h>
#include <airmap/date_time.h>
#include <airmap/geometry.h>
#include <airmap/pilot.h>

#include <cstdint>

namespace airmap {

struct Flight {
  using Id = std::string;

  Id id;
  Pilot pilot;
  Aircraft aircraft;
  float latitude;
  float longitude;
  float max_altitude;
  Geometry geometry;
  DateTime created_at;
  DateTime start_time;
  DateTime end_time;
};

}  // namespace airmap

#endif  // AIRMAP_FLIGHT_H_
