#ifndef AIRMAP_AIRSPACES_H_
#define AIRMAP_AIRSPACES_H_

#include <airmap/airspace.h>
#include <airmap/date_time.h>
#include <airmap/do_not_copy_or_move.h>
#include <airmap/outcome.h>

#include <functional>
#include <vector>

namespace airmap {

class Airspaces : DoNotCopyOrMove {
 public:
  struct ForIds {
    struct Parameters {
      Airspace::Id id;
    };

    using Result   = Outcome<std::vector<Airspace>, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct Search {
    struct Parameters {
      Optional<Airspace::Type> types;
      Optional<Airspace::Type> ignored_types;
      Optional<bool> full;
      Geometry geometry;
      Optional<std::uint32_t> buffer;
      Optional<std::uint32_t> limit;
      Optional<std::uint32_t> offset;
      Optional<DateTime> date_time;
    };

    using Result   = Outcome<std::vector<Airspace>, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  /// search queries the AirMap services for surrounding airspaces and
  /// reports back the results to 'cb'.
  virtual void search(const Search::Parameters& parameters, const Search::Callback& cb) = 0;

  /// for_ids queries the AirMap services for detailed information about
  /// airspaces identified by UUIDs and reports back results to 'cb'.
  virtual void for_ids(const ForIds::Parameters& parameters, const ForIds::Callback& cb) = 0;

 protected:
  Airspaces() = default;
};

}  // namespace airmap

#endif  // AIRMAP_AIRSPACES_H_
