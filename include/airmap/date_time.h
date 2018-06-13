#ifndef AIRMAP_DATE_TIME_H_
#define AIRMAP_DATE_TIME_H_

#include <memory>
#include <string>

namespace airmap {

class Clock;
class DateTime;
class TimeDuration;
class Hours;
class Minutes;
class Seconds;
class Milliseconds;
class Microseconds;

/// milliseconds_since_epoch returns the milliseconds that elapsed since the UNIX epoch.
std::uint64_t milliseconds_since_epoch(const DateTime& dt);
/// microseconds_since_epoch returns the microseconds that elapsed since the UNIX epoch.
std::uint64_t microseconds_since_epoch(const DateTime& dt);
/// from_seconds_since_epoch returns a DateTime.
DateTime from_seconds_since_epoch(const Seconds& s);
/// from_milliseconds_since_epoch returns a DateTime.
DateTime from_milliseconds_since_epoch(const Milliseconds& ms);
/// from_microseconds_since_epoch returns a DateTime.
DateTime from_microseconds_since_epoch(const Microseconds& us);

// moves the datetime forward to the specified hour
DateTime move_to_hour(const DateTime& dt, int hour);

/// Clock marks the reference for time measurements.
class Clock {
 public:
  static DateTime universal_time();
  static DateTime local_time();
};

/// DateTime marks a specific point in time, in reference to Clock.
class DateTime {
 public:
  DateTime();
  DateTime(const std::string& time_iso);
  ~DateTime();
  DateTime(DateTime const& old);
  DateTime &operator=(const DateTime &);
  DateTime &operator=(DateTime &&);

  DateTime operator+(const Hours &) const;
  DateTime operator+(const Minutes &) const;
  DateTime operator+(const Seconds &) const;
  DateTime operator+(const Milliseconds &) const;
  DateTime operator+(const Microseconds &) const;
  TimeDuration operator-(const DateTime &) const;
  bool operator==(const DateTime &) const;
  bool operator!=(const DateTime &) const;

  friend std::istream &operator>>(std::istream &, DateTime &);
  friend std::ostream &operator<<(std::ostream &, const DateTime &);

  std::string to_iso_string() const;

  DateTime date() const;
  TimeDuration time_of_day() const;

 private:
  struct Impl;
  std::unique_ptr<Impl> impl;
};

class TimeDuration {
 public:
  TimeDuration();
  ~TimeDuration();
  TimeDuration(TimeDuration const& old);
  TimeDuration &operator=(const TimeDuration &);
 
  long total_seconds() const;
  long total_milliseconds() const;
  long total_microseconds() const;

  long hours() const;

private:
  struct Impl;
  std::unique_ptr<Impl> impl;

  friend TimeDuration DateTime::operator-(const DateTime &) const;
  friend TimeDuration DateTime::time_of_day() const;
};

struct Hours {
    long hours;
};

struct Minutes {
    long minutes;
};

struct Seconds {
    long seconds;
};

struct Milliseconds {
    long milliseconds;
};

struct Microseconds {
    long microseconds;
};

namespace iso8601 {

/// parse parses a DateTime instance from the string s in iso8601 format.
DateTime parse(const std::string& s);
/// generate returns a string in iso8601 corresponding to 'dt'.
std::string generate(const DateTime& dt);

}  // namespace iso8601

}  // namespace airmap

#endif  // AIRMAP_DATE_TIME_H_
