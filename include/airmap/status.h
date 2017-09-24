#ifndef AIRMAP_STATUS_H_
#define AIRMAP_STATUS_H_

#include <airmap/airspace.h>
#include <airmap/date_time.h>
#include <airmap/do_not_copy_or_move.h>
#include <airmap/geometry.h>
#include <airmap/optional.h>
#include <airmap/outcome.h>

#include <cstdint>
#include <functional>
#include <iosfwd>
#include <string>
#include <vector>

namespace airmap {

/// Status provides functionality to query airspace and weather information about
/// a geographic area.
class Status : DoNotCopyOrMove {
 public:
  /// Color enumerates known colors assigned to advisories.
  enum class Color { green = 0, yellow = 1, orange = 2, red = 3 };

  /// Advisory bundles together airspace information and its evaluation in terms
  /// good to fly/needs information or feedback/conflict.
  struct Advisory {
    Airspace airspace;  /// The airspace that the advisory refers to.
    Color color;        /// The evaluation of the airspace.
  };

  /// Wind bundles up attributes describing a wind conditions.
  struct Wind {
    std::uint32_t heading = 0;
    std::uint32_t speed   = 0;
    std::uint32_t gusting = 0;
  };

  /// Weather bundles up attributes describing a weather condition.
  struct Weather {
    std::string condition;
    std::string icon;
    Wind wind;
    std::int32_t temperature    = 0;
    float humidity              = 0.0;
    std::uint32_t visibility    = 0;
    std::uint32_t precipitation = 0;
  };

  /// Report summarizes information about a geographic area.
  struct Report {
    std::uint32_t max_safe_distance = 0;
    Color advisory_color;
    std::vector<Advisory> advisories;
    Weather weather;
  };

  /// GetStatus bundles up types to ease interaction
  /// with Status::get_status*.
  struct GetStatus {
    struct Parameters {
      Required<float> latitude;
      Required<float> longitude;
      Optional<Airspace::Type> types;
      Optional<Airspace::Type> ignored_types;
      Optional<bool> weather;
      Optional<DateTime> flight_date_time;
      Optional<Geometry> geometry;
      Optional<std::uint32_t> buffer;
    };
    using Result   = Outcome<Report, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  /// get_status searches flight advisories for 'parameters' and reports
  /// results back to 'cb'.
  virtual void get_status_by_point(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) = 0;

  /// get_status searches flight advisories for 'parameters' and reports
  /// results back to 'cb'.
  virtual void get_status_by_path(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) = 0;

  /// get_status searches flight advisories for 'parameters' and reports
  /// results back to 'cb'.
  virtual void get_status_by_polygon(const GetStatus::Parameters& parameters, const GetStatus::Callback& cb) = 0;

 protected:
  Status() = default;
};

/// @cond
std::ostream& operator<<(std::ostream& out, Status::Color color);
std::istream& operator>>(std::istream& in, Status::Color& color);
/// @endcond

}  // namespace airmap

#endif  // AIRMAP_STATUS_H_
