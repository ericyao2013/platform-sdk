#ifndef AIRMAP_MAVLINK_HEARTBEAT_H_
#define AIRMAP_MAVLINK_HEARTBEAT_H_

#include <standard/mavlink.h>

#include <iosfwd>

namespace airmap {
namespace mavlink {

using Heartbeat = mavlink_heartbeat_t;

}  // namespace mavlink
}  // namespace airmap

std::ostream& operator<<(std::ostream& out, const airmap::mavlink::Heartbeat& heartbeat);

#endif  // AIRMAP_MAVLINK_HEARTBEAT_H_