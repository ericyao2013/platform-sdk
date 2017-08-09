#include <airmap/mavlink/vehicle_tracker.h>

void airmap::mavlink::VehicleTracker::update(const mavlink_message_t& msg) {
  auto it = vehicles_.find(msg.sysid);

  if (it == vehicles_.end())
    std::tie(it, std::ignore) = vehicles_.emplace(msg.sysid, std::make_shared<Vehicle>(msg.sysid));

  for (const auto& monitor : monitors_)
    monitor->on_vehicle_added(it->second);

  it->second->update(msg);
}

void airmap::mavlink::VehicleTracker::register_monitor(const std::shared_ptr<Monitor>& monitor) {
  monitors_.insert(monitor);
}

void airmap::mavlink::VehicleTracker::unregister_monitor(const std::shared_ptr<Monitor>& monitor) {
  monitors_.erase(monitor);
}