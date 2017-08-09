#ifndef AIRMAP_MAVLINK_VEHICLE_TRACKER_H_
#define AIRMAP_MAVLINK_VEHICLE_TRACKER_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/mavlink/vehicle.h>

#include <standard/mavlink.h>

#include <memory>
#include <unordered_map>
#include <unordered_set>

namespace airmap {
namespace mavlink {

class VehicleTracker {
 public:
  class Monitor : DoNotCopyOrMove {
   public:
    virtual void on_vehicle_added(const std::shared_ptr<Vehicle>& vehicle)   = 0;
    virtual void on_vehicle_removed(const std::shared_ptr<Vehicle>& vehicle) = 0;

   protected:
    Monitor() = default;
  };

  void update(const mavlink_message_t& msg);

  void register_monitor(const std::shared_ptr<Monitor>& monitor);
  void unregister_monitor(const std::shared_ptr<Monitor>& monitor);

 protected:
  std::unordered_set<std::shared_ptr<Monitor>> monitors_;
  std::unordered_map<std::uint8_t, std::shared_ptr<Vehicle>> vehicles_;
};

}  // namespace mavlink
}  // namespace airmap

#endif  // AIRMAP_MAVLINK_VEHICLE_TRACKER_H_