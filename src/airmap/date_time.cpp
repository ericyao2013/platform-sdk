#include <airmap/date_time.h>

#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <memory>

namespace airmap {

DateTime Clock::universal_time() {
    const auto ptime(boost::posix_time::microsec_clock::universal_time());
    return DateTime(boost::posix_time::to_iso_string(ptime));
}

DateTime Clock::local_time() {
    const auto ptime(boost::posix_time::microsec_clock::local_time());
    return DateTime(boost::posix_time::to_iso_string(ptime));
}

struct TimeDuration::Impl {
    boost::posix_time::time_duration time_duration;
};

TimeDuration::TimeDuration()
    : impl(std::unique_ptr<Impl>(new Impl())) {}
TimeDuration::TimeDuration(TimeDuration const& old)
    : impl(std::unique_ptr<Impl>(new Impl{old.impl->time_duration})) {}

long TimeDuration::total_seconds() const {
    return impl->time_duration.total_seconds();
}

long TimeDuration::total_microseconds() const {
    return impl->time_duration.total_microseconds();
}

long TimeDuration::total_milliseconds() const {
    return impl->time_duration.total_milliseconds();
}

long TimeDuration::hours() const {
    return impl->time_duration.hours();
}

TimeDuration::~TimeDuration() = default;

struct DateTime::Impl {
    boost::posix_time::ptime ptime;
};

DateTime::DateTime()
    : impl(std::unique_ptr<Impl>(new Impl())) {} 
DateTime::DateTime(DateTime const& old)
    : impl(std::unique_ptr<Impl>(new Impl{old.impl->ptime})) {}
DateTime::DateTime(DateTime && old)
    : impl(std::move(old.impl)) {}
DateTime::DateTime(const std::string& iso_time)
    : impl(std::unique_ptr<Impl>(new Impl{boost::posix_time::from_iso_string(iso_time)})) {}

DateTime::~DateTime() = default;

DateTime& DateTime::operator=(const DateTime& other) {
    if (this != &other) {
        impl = std::unique_ptr<Impl>(new Impl{other.impl->ptime});
    }

    return *this;
}

DateTime& DateTime::operator=(DateTime&& other) {
    impl = std::move(other.impl);
    return *this;
}

DateTime DateTime::operator+(const Hours& other) const {
    DateTime date_time;
    date_time.impl->ptime = this->impl->ptime + boost::posix_time::hours{other.hours};

    return date_time;
}

DateTime DateTime::operator+(const Minutes& other) const {
    DateTime date_time;
    date_time.impl->ptime = this->impl->ptime + boost::posix_time::minutes{other.minutes};

    return date_time;
}

DateTime DateTime::operator+(const Seconds& other) const {
    DateTime date_time;
    date_time.impl->ptime = this->impl->ptime + boost::posix_time::seconds{other.seconds};

    return date_time;
}

DateTime DateTime::operator+(const Milliseconds& other) const {
    DateTime date_time;
    date_time.impl->ptime = this->impl->ptime + boost::posix_time::milliseconds{other.milliseconds};

    return date_time;
}

DateTime DateTime::operator+(const Microseconds& other) const {
    DateTime date_time;
    date_time.impl->ptime = this->impl->ptime + boost::posix_time::microseconds{other.microseconds};

    return date_time;
}

TimeDuration DateTime::operator-(const DateTime& other) const {
    TimeDuration time_duration;
    time_duration.impl->time_duration = this->impl->ptime - other.impl->ptime;

    return time_duration;
}

bool DateTime::operator==(const DateTime& other) const {
    return this->impl->ptime == other.impl->ptime;
}

bool DateTime::operator!=(const DateTime& other) const {
    return !(*this == other);
}

std::istream& operator>>(std::istream& from, DateTime& to)
{
    from >> to.impl->ptime;
    return from;
}

std::ostream& operator<<(std::ostream& to, const DateTime& from)
{
    to << from.impl->ptime;
    return to;
}

std::string DateTime::to_iso_string() const {
    return boost::posix_time::to_iso_string(impl->ptime);
}

DateTime DateTime::date() const {
    DateTime dt;
    dt.impl->ptime = boost::posix_time::ptime(this->impl->ptime.date());

    return dt;
}

TimeDuration DateTime::time_of_day() const {
    TimeDuration time_duration;
    time_duration.impl->time_duration = this->impl->ptime.time_of_day();

    return time_duration;
}

} // namespace airmap

namespace {
constexpr const char* format{"%Y-%m-%dT%H:%M:%sZ"};
const airmap::DateTime epoch = airmap::DateTime("19700101T000000,000000000");
}  // namespace

std::uint64_t airmap::milliseconds_since_epoch(const DateTime& dt) {
  return (dt - epoch).total_milliseconds();
}

std::uint64_t airmap::microseconds_since_epoch(const DateTime& dt) {
  return (dt - epoch).total_microseconds();
}

airmap::DateTime airmap::from_seconds_since_epoch(const Seconds& s) {
  return epoch + s;
}

airmap::DateTime airmap::from_milliseconds_since_epoch(const Milliseconds& ms) {
  return epoch + ms;
}

airmap::DateTime airmap::from_microseconds_since_epoch(const Microseconds& us) {
  return epoch + us;
}

airmap::DateTime airmap::move_to_hour(const DateTime& dt, int hour) {
  return dt + Hours{((hour > dt.time_of_day().hours()) ? 0 : 24) + hour - dt.time_of_day().hours()};
}

airmap::DateTime airmap::iso8601::parse(const std::string& s) {
  boost::posix_time::time_input_facet facet{format, 1};

  std::istringstream iss{s};
  iss.imbue(std::locale{iss.getloc(), &facet});
  boost::posix_time::ptime result;
  iss >> result;

  const std::string iso_time(boost::posix_time::to_iso_string(result));
  DateTime dt(iso_time);
  return dt;
}

std::string airmap::iso8601::generate(const DateTime& dt) {
  boost::posix_time::time_facet facet(1);
  facet.format(format);

  std::ostringstream oss;
  oss.imbue(std::locale(oss.getloc(), &facet));

  boost::posix_time::ptime boost_dt = boost::posix_time::from_iso_string(dt.to_iso_string());
  oss << boost_dt;

  return oss.str();
}
