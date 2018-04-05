#ifndef AIRMAP_ELEVATION_H_
#define AIRMAP_ELEVATION_H_

#include <airmap/airspace.h>
#include <airmap/date_time.h>
#include <airmap/do_not_copy_or_move.h>
#include <airmap/error.h>
#include <airmap/geometry.h>
#include <airmap/optional.h>
#include <airmap/outcome.h>

#include <cstdint>
#include <functional>
#include <iosfwd>
#include <string>
#include <vector>

namespace airmap {

/// Elevation provides elevation data for all locations on the surface of the earth at a resolution of 10 meters.
class Elevation : DoNotCopyOrMove {
 public:

  /// GetElevation bundles up types to ease interaction
  /// with Elevation::get_elevation*.
  struct GetElevation {
    /// Parameters bundles up input parameters.
    struct Parameters {
      std::vector<float> points; ///< Array of latitude/longitude points.
    };
    /// Result models the outcome of calling Status::get_status*.
    using Result = Outcome<std::vector<float>, Error>;
    /// Callback describes the function signature of the callback that is
    /// invoked when a call to Status::get_status* finishes.
    using Callback = std::function<void(const Result&)>;
  };

  /// get_elevation searches elevations for 'parameters' and reports
  /// results back to 'cb'.
  virtual void get_elevation_points(const GetElevation::Parameters& parameters, const GetElevation::Callback& cb) = 0;

 protected:
  /// @cond
  Elevation() = default;
  /// @endcond
};

}  // namespace airmap

#endif  // AIRMAP_ELEVATION_H_
