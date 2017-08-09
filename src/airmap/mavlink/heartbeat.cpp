#include <airmap/mavlink/heartbeat.h>

#include <iostream>

std::ostream& operator<<(std::ostream& out, const airmap::mavlink::Heartbeat& heartbeat) {
  return out << "(" << heartbeat.type << "," << heartbeat.autopilot << "," << heartbeat.base_mode << ","
             << heartbeat.custom_mode << "," << heartbeat.system_status << "," << heartbeat.mavlink_version << ")";
}