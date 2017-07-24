#ifndef AIRMAP_AIRCRAFTS_H_
#define AIRMAP_AIRCRAFTS_H_

#include <airmap/aircraft.h>
#include <airmap/do_not_copy_or_move.h>
#include <airmap/optional.h>
#include <airmap/outcome.h>

#include <functional>
#include <string>
#include <vector>

namespace airmap {

class Aircrafts : DoNotCopyOrMove {
 public:
  struct Manufacturers {
    struct Parameters {
      Optional<std::string> manufacturer_name;
    };

    using Result   = Outcome<std::vector<Aircraft::Manufacturer>, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct Models {
    struct Parameters {
      Optional<Aircraft::Manufacturer> manufacturer;
      Optional<std::string> model_name;
    };

    using Result   = Outcome<std::vector<Aircraft>, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  struct ModelForId {
    struct Parameters {
      std::string id;
    };

    using Result   = Outcome<Aircraft, std::exception_ptr>;
    using Callback = std::function<void(const Result&)>;
  };

  Aircrafts(const Aircrafts&) = delete;
  Aircrafts(Aircrafts&&)      = delete;
  virtual ~Aircrafts()        = default;
  Aircrafts& operator=(const Aircrafts&) = delete;
  Aircrafts& operator=(Aircrafts&&) = delete;

  /// manufacturers queries the AirMap services for known aircraft
  /// manufacturers, reporting results to 'cb'.
  virtual void manufacturers(const Manufacturers::Parameters& parameters,
                             const Manufacturers::Callback& cb) = 0;

  /// models queries the AirMap services for detailed information about
  /// known Aircraft models and reports back results to 'cb'.
  virtual void models(const Models::Parameters& parameters, const Models::Callback& cb) = 0;

  /// models queries the AirMap services for detailed information about
  /// an Aircraft model identified by 'ModelForId::Parameters::id' and reports back results to 'cb'.
  virtual void model_for_id(const ModelForId::Parameters& parameters,
                            const ModelForId::Callback& cb) = 0;

 protected:
  Aircrafts() = default;
};

}  // namespace airmap

#endif  // AIRMAP_AIRCRAFTS_H_
