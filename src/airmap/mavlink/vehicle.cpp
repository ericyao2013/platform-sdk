#include <airmap/mavlink/vehicle.h>

#include <common/mavlink_msg_global_position_int.h>
#include <common/mavlink_msg_heartbeat.h>

#include <cassert>

airmap::mavlink::Vehicle::Vehicle(std::uint8_t system_id) : system_id_{system_id} {
}

void airmap::mavlink::Vehicle::update(const mavlink_message_t& msg) {
  assert(system_id_ == msg.sysid);

  switch (msg.msgid) {
    case MAVLINK_MSG_ID_HEARTBEAT:
      handle_msg_heartbeat(msg);
      break;
    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
      handle_msg_global_position_int(msg);
      break;
  }
}

void airmap::mavlink::Vehicle::register_monitor(const std::shared_ptr<Monitor>& monitor) {
  monitors_.insert(monitor);
}

void airmap::mavlink::Vehicle::unregister_monitor(const std::shared_ptr<Monitor>& monitor) {
  monitors_.erase(monitor);
}

void airmap::mavlink::Vehicle::handle_msg_heartbeat(const mavlink_message_t& msg) {
  mavlink_heartbeat_t hb;
  mavlink_msg_heartbeat_decode(&msg, &hb);

  if (system_status_ != hb.system_status) {
    for (const auto& monitor : monitors_) {
      monitor->on_system_status_changed(system_status_, static_cast<MAV_STATE>(hb.system_status));
    }
    system_status_ = static_cast<State>(hb.system_status);
  }
}

void airmap::mavlink::Vehicle::handle_msg_global_position_int(const mavlink_message_t& msg) {
  GlobalPositionInt gpi;
  mavlink_msg_global_position_int_decode(&msg, &gpi);

  for (const auto& monitor : monitors_) {
    monitor->on_position_changed(global_position_int_, gpi);
  }
  global_position_int_ = gpi;
}