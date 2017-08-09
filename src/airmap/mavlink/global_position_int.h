#ifndef AIRMAP_MAVLINK_GLOBAL_POSITION_INT_H_
#define AIRMAP_MAVLINK_GLOBAL_POSITION_INT_H_

#include <standard/mavlink.h>

#include <iosfwd>

namespace airmap {
namespace mavlink {

using GlobalPositionInt = mavlink_global_position_int_t;

}  // namespace mavlink
}  // namespace airmap

std::ostream& operator<<(std::ostream& out, const airmap::mavlink::GlobalPositionInt& position);

#endif  // AIRMAP_MAVLINK_GLOBAL_POSITION_INT_H_