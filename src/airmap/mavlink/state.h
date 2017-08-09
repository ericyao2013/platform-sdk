#ifndef AIRMAP_MAVLINK_STATE_H_
#define AIRMAP_MAVLINK_STATE_H_

#include <standard/mavlink.h>

#include <iosfwd>

namespace airmap {
namespace mavlink {

using State = MAV_STATE;

}  // namespace mavlink
}  // namespace airmap

std::ostream& operator<<(std::ostream& out, airmap::mavlink::State state);

#endif  // AIRMAP_MAVLINK_STATE_H_