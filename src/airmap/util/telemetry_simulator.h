#ifndef AIRMAP_UTIL_TELEMETRY_SIMULATOR_H_
#define AIRMAP_UTIL_TELEMETRY_SIMULATOR_H_

#include <airmap/geometry.h>
#include <airmap/util/cheap_ruler.h>

#include <chrono>

namespace airmap {
namespace util {

class TelemetrySimulator {
 public:
  explicit TelemetrySimulator(const airmap::Geometry::Polygon& polygon);

  // Updates the state of the simulator and calculates a new position.
  Geometry::Coordinate update();

 private:
  using Clock = std::chrono::high_resolution_clock;

  airmap::Geometry::Polygon polygon_;
  std::vector<airmap::Geometry::Coordinate>::iterator segment_begin_;
  std::vector<airmap::Geometry::Coordinate>::iterator segment_end_;
  CheapRuler segment_ruler_;
  Clock::time_point segment_start_time_;
  double segment_bearing_;
  double segment_length_;
  airmap::Geometry::Coordinate current_data_;
  Clock::time_point current_time_;
};

}  // namespace util
}  // namespace airmap

#endif  // AIRMAP_UTIL_TELEMETRY_SIMULATOR_H_
