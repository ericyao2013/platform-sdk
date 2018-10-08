#ifndef AIRMAP_ELEVATION_H_
#define AIRMAP_ELEVATION_H_

#include <airmap/airspace.h>
#include <airmap/date_time.h>
#include <airmap/do_not_copy_or_move.h>
#include <airmap/error.h>
#include <airmap/geometry.h>
#include <airmap/optional.h>
#include <airmap/outcome.h>
#include <airmap/visibility.h>

#include <cstdint>
#include <functional>
#include <iosfwd>
#include <string>
#include <vector>

namespace airmap {

/// Elevation provides elevation data for all locations on the surface of the earth at a resolution of 10 meters.
class AIRMAP_EXPORT Elevation : DoNotCopyOrMove {
 public:
  /// Bounds.
  struct AIRMAP_EXPORT Bounds {
    std::vector<float> sw;
    std::vector<float> ne;
  };

  /// Stats.
  struct AIRMAP_EXPORT Stats {
    std::int32_t max;
    std::int32_t min;
    float avg;
  };

  /// Carpet.
  struct AIRMAP_EXPORT Carpet {
    Bounds bounds;
    Stats stats;
    std::vector<std::vector<float>> carpet;
  };

  /// Path.
  struct AIRMAP_EXPORT Path {
    std::vector<float> from;
    std::vector<float> to;
    std::vector<float> step;
    std::vector<std::int32_t> profile;
  };

  /// GetElevationPoints bundles up types to ease interaction
  /// with Elevation::get_elevation*.
  struct AIRMAP_EXPORT GetElevationPoints {
    /// Parameters bundles up input parameters.
    struct Parameters {
      Required<std::string> points;  ///< Array of latitude/longitude points.
    };
    /// Result models the outcome of calling Elevation::get_elevation*.
    using Result = Outcome<std::vector<std::int32_t>, Error>;
    /// Callback describes the function signature of the callback that is
    /// invoked when a call to Elevation::get_elevation* finishes.
    using Callback = std::function<void(const Result&)>;
  };

  /// GetElevationCarpet bundles up types to ease interaction
  /// with Elevation::get_elevation*.
  struct AIRMAP_EXPORT GetElevationCarpet {
    /// Parameters bundles up input parameters.
    struct Parameters {
      Required<std::string> points;  ///< Array of latitude/longitude points.
    };
    /// Result models the outcome of calling Elevation::get_elevation*.
    using Result = Outcome<Carpet, Error>;
    /// Callback describes the function signature of the callback that is
    /// invoked when a call to Elevation::get_elevation* finishes.
    using Callback = std::function<void(const Result&)>;
  };

  /// GetElevationPath bundles up types to ease interaction
  /// with Elevation::get_elevation*.
  struct AIRMAP_EXPORT GetElevationPath {
    /// Parameters bundles up input parameters.
    struct Parameters {
      Required<std::string> points;  ///< Array of latitude/longitude points.
    };
    /// Result models the outcome of calling Elevation::get_elevation*.
    using Result = Outcome<std::vector<Path>, Error>;
    /// Callback describes the function signature of the callback that is
    /// invoked when a call to Elevation::get_elevation* finishes.
    using Callback = std::function<void(const Result&)>;
  };


  /// get_elevation_points searches elevations for 'parameters' and reports
  /// results back to 'cb'.
  virtual void get_elevation_points(const GetElevationPoints::Parameters& parameters, const GetElevationPoints::Callback& cb) = 0;

  /// get_elevation_carpet searches elevations for 'parameters' and reports
  /// results back to 'cb'.
  //virtual void get_elevation_carpet(const GetElevationCarpet::Parameters& parameters, const GetElevationCarpet::Callback& cb) = 0;

  /// get_elevation_path searches elevations for 'parameters' and reports
  /// results back to 'cb'.
  //virtual void get_elevation_path(const GetElevationPath::Parameters& parameters, const GetElevationPath::Callback& cb) = 0;

 protected:
  /// @cond
  Elevation() = default;
  /// @endcond
};

}  // namespace airmap

#endif  // AIRMAP_ELEVATION_H_
