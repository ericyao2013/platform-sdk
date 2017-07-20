#ifndef AIRMAP_DATE_TIME_H_
#define AIRMAP_DATE_TIME_H_

#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <cstdint>
#include <memory>
#include <string>

namespace airmap {

using Clock = boost::posix_time::second_clock;
using DateTime = boost::posix_time::ptime;
using Hours = boost::posix_time::hours;
using Minutes = boost::posix_time::minutes;
using Seconds = boost::posix_time::seconds;

std::uint64_t milliseconds_since_epoch(const DateTime& dt);

namespace iso8601 {

DateTime parse(const std::string& s);
std::string generate(const DateTime& dt);

}  // namespace iso8601

}  // namespace airmap

#endif  // AIRMAP_DATE_TIME_H_
