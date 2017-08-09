#ifndef AIRMAP_MAVLINK_VEHICLE_H_
#define AIRMAP_MAVLINK_VEHICLE_H_

#include <airmap/do_not_copy_or_move.h>
#include <airmap/optional.h>

#include <airmap/mavlink/global_position_int.h>
#include <airmap/mavlink/state.h>

#include <cstdint>

#include <memory>
#include <unordered_set>

namespace airmap {
namespace mavlink {

class Vehicle {
 public:
  class Monitor : DoNotCopyOrMove {
   public:
    virtual void on_system_status_changed(const Optional<State>& old_state, State new_state) = 0;
    virtual void on_position_changed(const Optional<GlobalPositionInt>& old_position,
                                     const GlobalPositionInt& new_position)                  = 0;

   protected:
    Monitor() = default;
  };

  explicit Vehicle(std::uint8_t system_id);

  void update(const mavlink_message_t& msg);
  void register_monitor(const std::shared_ptr<Monitor>& monitor);
  void unregister_monitor(const std::shared_ptr<Monitor>& monitor);

 protected:
  void handle_msg_global_position_int(const mavlink_message_t& msg);
  void handle_msg_heartbeat(const mavlink_message_t& msg);

  std::uint8_t system_id_;
  std::unordered_set<std::shared_ptr<Monitor>> monitors_;

  State system_status_{MAV_STATE_UNINIT};
  Optional<GlobalPositionInt> global_position_int_;
};

}  // namespace mavlink
}  // namespace airmap

#endif  // AIRMAP_MAVLINK_VEHICLE_H_